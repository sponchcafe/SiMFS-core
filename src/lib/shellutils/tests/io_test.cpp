#include "io_fixtures.hpp"
#include "shellutils/io.hpp"
#include <fstream>
#include <array>

using namespace sim::io;


//---------------------------------------------------------------------------//
TEST(IOBasicTest, TestWriteRead){
    std::ofstream f = std::ofstream("./testfile", std::ofstream::binary);
    ASSERT_EQ(f.good(), true);
    for (int i = 0; i< 20000; i++) write_binary<int>(f, i);
    f.close();
    int x = 0;
    std::ifstream i = std::ifstream("./testfile", std::ifstream::binary);
    while(!i.fail()) read_binary<int>(i, x);
    ASSERT_EQ(x, 19999);
    std::remove("./testfile");
}


//---------------------------------------------------------------------------//
TEST_F(ReadTest, Creation){
    auto in_file = Input<int>(bytefile);
    auto in_file_sized = Input<int>(bytefile, 100);
}


//---------------------------------------------------------------------------//
TEST_F(ReadTest, ReadFileSmallerBuffer){
    Input<char> in = Input<char>(bytefile, 32);
    while(in.get(c)){
        count++;
        ASSERT_EQ(c, 7);
    }
    ASSERT_EQ(count, 1000);
}

//---------------------------------------------------------------------------//
TEST_F(ReadTest, ReadFileSameSizeBuffer){
    Input<char> in = Input<char>(bytefile, 1000);
    while(in.get(c)){
        count++;
        ASSERT_EQ(c, 7);
    }
    ASSERT_EQ(count, 1000);
}

//---------------------------------------------------------------------------//
TEST_F(ReadTest, ReadFileLargerBuffer){
    Input<char> in = Input<char>(bytefile, 4096);
    while(in.get(c)){
        count++;
        ASSERT_EQ(c, 7);
    }
    ASSERT_EQ(count, 1000);
}


//---------------------------------------------------------------------------//
TEST_F(ReadTest, Nofile){
    Input<char> in = Input<char>("nothere");
    while(in.get(c)){
        count++;
    }
    ASSERT_EQ(count, 0);
}


//---------------------------------------------------------------------------//
TEST_F(ReadTest, TypedRead){
    double d = 0.0;
    Input<double> in = Input<double>(bytefile);
    while(in.get(d)) count++;

    // create the double represented by 8 0x07 bytes.
    char bytes[8] = {
        dummy_char, dummy_char, dummy_char, dummy_char,
        dummy_char, dummy_char, dummy_char, dummy_char
    };
    double const * val = reinterpret_cast<double const *>(bytes);

    ASSERT_EQ(count, 1000/sizeof(double));
    ASSERT_EQ(d, *val);
}


//---------------------------------------------------------------------------//
TEST(WriteTest, Creation){
   auto out_default = Output<int>();
   auto out_sized = Output<int>(100);
   auto out_file = Output<int>("filename.dat");
   auto out_file_sized = Output<int>("filename.dat", 100);
   std::remove("filename.dat");
}


//---------------------------------------------------------------------------//
TEST(WriteTest, WriteStdoutSmallerBuffer){
    std::string expected;
    char c = 0x07;
    {
        Output<char> out = Output<char>(256);
        testing::internal::CaptureStdout();
        for (int i=0; i<1000; i++){
            out.put(c); 
            expected += c;
        }
    }
    std::string result = testing::internal::GetCapturedStdout();
    ASSERT_EQ(result, expected);
}


//---------------------------------------------------------------------------//
TEST(WriteTest, WritStdoutSameSizeBuffer){
    std::string expected;
    char c = 0x07;
    {
        Output<char> out = Output<char>(1000);
        testing::internal::CaptureStdout();
        for (int i=0; i<1000; i++){
            out.put(c); 
            expected += c;
        }
    }
    std::string result = testing::internal::GetCapturedStdout();
    ASSERT_EQ(result, expected);
}


//---------------------------------------------------------------------------//
TEST(WriteTest, WriteStdoutLargerBuffer){
    std::string expected;
    char c = 0x07;
    {
        Output<char> out = Output<char>(4096);
        testing::internal::CaptureStdout();
        for (int i=0; i<1000; i++){
            out.put(c); 
            expected += c;
        }
    }
    std::string result = testing::internal::GetCapturedStdout();
    ASSERT_EQ(result, expected);
}


//---------------------------------------------------------------------------//
TEST(WriteTest, TypedWrite){
    std::string expected;
    // building a double
    char dummy_char = 0x07;
    char bytes[8] = {
        dummy_char, dummy_char, dummy_char, dummy_char,
        dummy_char, dummy_char, dummy_char, dummy_char
    };
    double * val = reinterpret_cast<double *>(bytes);

    {
        Output<double> out = Output<double>(8);
        testing::internal::CaptureStdout();
        for (int i=0; i<10; i++){
            out.put(*val); 
            for (int j=0; j<8; j++){
               expected += dummy_char;
            }
        }
    }
    std::string result = testing::internal::GetCapturedStdout();
    ASSERT_EQ(result, expected);
}


//---------------------------------------------------------------------------//
TEST(ReproductionTest, WriteRead){

    char const * file = ".TEST_TRANSFER_FILE.dat";
    std::vector<double> expected;
    std::vector<double> result;

    // Writing
    {
        Output<double> out = Output<double>(file);
        for (int i=0; i<10; i++){
            double value = 0.05*i+0.003*i*i;
            out.put(value);
            expected.push_back(value);
        }
    } // Scoped to ensure destruction and flush of output

    // Reading
    {
        Input<double> in = Input<double>(file);
        double value = 0.0;
        while(in.get(value)) result.push_back(value);
    }

    ASSERT_EQ(expected, result);
    std::remove(file);

}
