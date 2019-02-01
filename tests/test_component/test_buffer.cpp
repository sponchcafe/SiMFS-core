#include "catch.hpp"
#include "io/buffer.hpp"
#include "definitions/types.hpp"
#include <vector>
#include <thread>
#include <fstream>


using namespace sim::io;

/*--------------------------------------------------------------------------*/
TEMPLATE_TEST_CASE("Output accepts data without blocking", "[output][types]",
        sim::Coordinate, sim::TimedValue, sim::realtime_t){

    GIVEN ("An output buffer"){

        BufferOutput<TestType> out{"out1"};

        WHEN ("Data is written"){

            TestType data{};
            out.put(data);

            THEN ("There is no blocking"){
                REQUIRE(true);
            }

        }

        WHEN ("A chunk of data is written"){

            std::vector<TestType> data{};
            for (int i=0; i<10; i++) data.push_back(TestType{});
            out.put_chunk(data);

            THEN ("There is no blocking"){
                REQUIRE(true);
            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Data from an output buffer is available from a corresponding input", "[input]"){

    GIVEN ("An output buffer with some data"){

        { // scoped output buffer
            BufferOutput<double> out{"buf1"};
            for (int i=0; i<100; i++) {
                double data = i*1.0;
                out.put(data);
            }
        } // end of stream is written

        WHEN ("An input of the same name and type is created"){

            BufferInput<double> in{"buf1"};

            THEN ("Peeking the input of the same type and name yields the first value"){

                REQUIRE(in.peek() == 0.0);

            }

            AND_THEN("Reading the next value from the input yields the first value"){

                double result = -1.0;
                in.get(result);
                REQUIRE(result == 0.0);

            }

            AND_THEN("Reading the whole stream until it evaluates to false yields all values"){

                int result_count = 0;
                double result = 0.0;
                while (in.get(result)) result_count++;
                REQUIRE(result_count == 100);

            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Outputs accept a chunk of data at once", "[input][output][chunk]"){

    GIVEN("An Output buffer with a chunk of data"){

        { // scoped output
            BufferOutput<double> out{"buf1"};
            std::vector<double> data{};
            for (int i=0; i<100; i++) data.push_back(i*1.0);
            out.put_chunk(data);
        } // end of stream

        WHEN ("An input of the same name and type is created"){

            BufferInput<double> in{"buf1"};

            THEN("Reading a chunk yields the whole chunk of data"){
                std::vector<double> result{};
                in.get_chunk(result);
                REQUIRE(result.size() == 100);
            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Input can be sorted according to their next values", "[input][sort]"){

    GIVEN("Two Output buffers with some data"){

        { // scoped output
            BufferOutput<double> out{"buf1"};
            double d = 3.1; 
            out.put(d);
        } // end of stream

        { // scoped output
            BufferOutput<double> out{"buf2"};
            double d = 2.7; 
            double e = 5.0;
            out.put(d);
            out.put(e);
        } // end of stream

        WHEN("Two corresponding inputs are created"){

            BufferInput<double> in1{"buf1"};
            BufferInput<double> in2{"buf2"};

            THEN("They ary comparable by '<' and ordered according to their next values"){
                REQUIRE(in2 < in1); // 2.7 < 3.1
                double val;
                in2.get(val);       // removing 2.7, next is 5.0
                REQUIRE(in1 < in2); // 3.1 < 5.0
            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Thread can write and read an output-input-pair in parallel", "[input][output][.benchmark]"){

    GIVEN("Two threads with an input and an output of the same type and id"){

        std::vector<double> result{};
        int size = GENERATE(1, 10, 100, 1<<10, 1<<20, 1<<27);

        std::thread out_thr{
            [size] () -> void {
                BufferOutput<double> out{"buf1"};
                for (int i=0; i<size; i++){
                    double data = i*1.0;
                    out.put(data);
                }
            }
        };

        std::thread in_thr{
            [size, &result] () -> void {
                BufferInput<double> in{"buf1"};
                double data = 0.0;
                while(in.get(data)){
                    result.push_back(data);
                }
            }
        };

        WHEN("Both threads are done"){

            in_thr.join();
            out_thr.join();

            THEN("The data has passed from one to the other"){
                REQUIRE(result.size() == size);
            }

        }

    }

}

/*--------------------------------------------------------------------------*/
// Read and write binary file helpers for filesystem tests
/*--------------------------------------------------------------------------*/
std::vector<double> read_file(std::string fname){

    std::ifstream ifs{fname, std::ifstream::binary};
    std::vector<double> data{};

    std::streampos fsize = 0;
    ifs.seekg(0, std::ios::end);
    fsize = ifs.tellg() - fsize;
    ifs.seekg(0); // back to start

    data.resize(fsize / sizeof(double));

    if (ifs) ifs.read(reinterpret_cast<char *>(data.data()), fsize);

    ifs.close();

    return data;

}

void write_file(std::string fname, std::vector<double> data){

    std::ofstream ofs{fname, std::ofstream::binary};

    if (ofs) ofs.write(reinterpret_cast<char const *>(data.data()), 
            data.size()*sizeof(double));

    ofs.close();

}

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
TEST_CASE("Output buffers can be written to the file system", "[file]"){

    std::string fname = "outfile.dat";
    std::string bufname = "buf1";

    GIVEN("A filename and an output buffer with some data"){

        { // scoped output
            BufferOutput<double> out{bufname};
            double d = 3.1; 
            out.put(d);
        } // end of stream

        WHEN("The buffer is written to the file by file name"){

            buffer2file<double>(bufname, fname);

            THEN("The data is found in the file"){

                auto data = read_file(fname);

                REQUIRE(data.size() == 1);
                REQUIRE(data[0] == 3.1);

            }

            remove("outfile.dat");

        }

        WHEN("The buffer is written to file without a filename argument"){

            buffer2file<double>(bufname);

            THEN("The data is found in the file named <bufname>"){

                auto data = read_file(bufname);

                REQUIRE(data.size() == 1);
                REQUIRE(data[0] == 3.1);

            }

            remove(bufname.c_str());

        }

        WHEN("A buffer2file thread is created for a buffer and a filename"){

            std::thread b2f_thr = buffer2file_thread<double>(bufname, fname);

            AND_WHEN("It is completed"){

                b2f_thr.join();

                THEN("The data is found in the file"){ 

                    auto data = read_file(fname);

                    REQUIRE(data.size() == 1);
                    REQUIRE(data[0] == 3.1);

                }

            }

            remove(fname.c_str());

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Data can be read from filesystem to an input", "[file]"){

    GIVEN("Some data in a file"){

        std::string fname = "indata.dat";
        std::string bufname = "buf1";
        std::vector<double> data{1,2,3,4,5};

        write_file(fname, data);

        WHEN("The file is read to a buffer by name"){

            file2buffer<double>(fname, bufname);

            THEN("The is made available in the input"){

                BufferInput<double> in{bufname};
                double val = 0;
                std::vector<double> result{};
                while(in.get(val)) result.push_back(val);

                REQUIRE(result.size() == 5);
                REQUIRE(result[0] == 1);
                REQUIRE(result[4] == 5);

            }

        }

        WHEN("The file is read without a buffer name"){

            file2buffer<double>(fname);

            THEN("The is made available in an input with the filename as id"){

                BufferInput<double> in{fname};
                double val = 0;
                std::vector<double> result{};
                while(in.get(val)) result.push_back(val);

                REQUIRE(result.size() == 5);
                REQUIRE(result[0] == 1);
                REQUIRE(result[4] == 5);

            }

        }

        WHEN("A file2buffer thread is created"){

            std::thread f2b_thr = file2buffer_thread<double>(fname, bufname);

            AND_WHEN("This thread completes"){

                f2b_thr.join();

                THEN("The data is made available the input"){

                    BufferInput<double> in{bufname};
                    double val = 0;
                    std::vector<double> result{};
                    while(in.get(val)) result.push_back(val);

                    REQUIRE(result.size() == 5);
                    REQUIRE(result[0] == 1);
                    REQUIRE(result[4] == 5);

                }

            }

        }

        remove(fname.c_str());

    }

}


TEST_CASE("Read and write utilities", "[helpers]"){

    GIVEN("A data vector, a result vector and a buffer id"){

        std::vector<double> data{1,2,3,4};
        std::vector<double> result{};
        std::string buffer_id{"buf"};

        WHEN("A reader and writer thread are created"){

            auto out_thr = vector2buffer_thread<double>(data, buffer_id);
            auto in_thr = buffer2vector_thread<double>(buffer_id, result);

            AND_WHEN("Both threads complete"){

                out_thr.join();
                in_thr.join();

                THEN("Data is copied to the result vector"){
                    REQUIRE(data.size() == result.size());
                }

            }



        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Inputs can be read in non blocking mode", "[.TODO][input]"){

    REQUIRE(false);

}



