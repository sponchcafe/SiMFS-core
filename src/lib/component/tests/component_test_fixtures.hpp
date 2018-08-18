#ifndef SIMFS_COMPONENT_FIXTURES_H
#define SIMFS_COMPONENT_FIXTURES_H

#include <thread>
#include "component_test_mocks.hpp"
#include <vector>
#include "gtest/gtest.h"

using namespace sim;

//--------------------------------------------------------------------------//
class ComponentIO : public ::testing::Test {
    
    protected:
        
        //------------------------------------------------------------------//
        ComponentIO () {
            for(size_t i = 0; i<payload_size; i++){
                payload.push_back(i);
            }
        }
    
        //------------------------------------------------------------------//
        void with_named_pipe_test(){

            ProducerComponent prod{};
            prod.set_output("prod");
            prod.set_data(payload);

            auto prod_thr = run_component<ProducerComponent>(prod, true);
            auto out_thr = io::buffer2file_thread<int>("prod", p1);
            auto in_thr = io::file2buffer_thread<int>(p1, "cons");
           
            std::thread cons_thr = std::thread{
                [&] () {
                    ConsumerComponent cons{};
                    cons.set_input("cons");
                    cons.init();
                    cons.run();
                    result = cons.get_data();
                }
            };

            prod_thr.join();
            in_thr.join();
            out_thr.join();
            cons_thr.join();
            
        }

        //------------------------------------------------------------------//
        void with_file_test(){

            ProducerComponent prod{};
            prod.set_output("prod");
            prod.set_data(payload);

            auto prod_thr = run_component<ProducerComponent>(prod, true);
            auto out_thr = io::buffer2file_thread<int>("prod", f1);
               
            prod_thr.join();
            out_thr.join();
            

            auto in_thr = io::file2buffer_thread<int>(f1, "cons");
           
            std::thread cons_thr = std::thread{
                [&] () {
                    ConsumerComponent cons{};
                    cons.set_input("cons");
                    cons.init();
                    cons.run();
                    result = cons.get_data();
                }
            };

            in_thr.join();
            cons_thr.join();

        }

        //------------------------------------------------------------------//
        void pure_in_memory_test(){

            ProducerComponent prod{};
            prod.set_output(p1);
            prod.set_data(payload);

            TransformComponent trans{};
            trans.set_input(p1);
            trans.set_output(p2);
            
            std::thread prod_thr = run_component<ProducerComponent>(prod, true);
            std::thread trans_thr = run_component<TransformComponent>(trans, true);
            std::thread cons_thr = std::thread{
                [&] () {
                    ConsumerComponent cons{};
                    cons.set_input(p2);
                    cons.init();
                    cons.run();
                    result = cons.get_data();
                }
            };

            prod_thr.join();
            trans_thr.join();
            cons_thr.join();

        }

        //------------------------------------------------------------------//
        void validate_result(){

            EXPECT_EQ(payload.size(), result.size());
            EXPECT_GT(result.size(), 0);

            auto it1 = payload.begin();
            auto it2 = result.begin();

            while (it1 != payload.end() && it2 != result.end()){
                EXPECT_EQ(*it1++, *it2++);
            }

        }

        //------------------------------------------------------------------//
        void SetUp() {

            // Create named pipes
            mkfifo(p1.c_str(), S_IRUSR | S_IWUSR);
            mkfifo(p2.c_str(), S_IRUSR | S_IWUSR);
        }

        //------------------------------------------------------------------//
        void TearDown() {
            // Remove named pipes
            remove(p1.c_str());
            remove(p2.c_str());

            // Remove created files
            remove(f1.c_str());
            remove(f2.c_str());

        }

        //------------------------------------------------------------------//
        std::string p1 = "/tmp/simfs_test_P1"; // Pipe 1
        std::string p2 = "/tmp/simfs_test_P2"; // Pipe 2
        std::string f1 = "/tmp/simfs_test_F1.dat"; // File 1
        std::string f2 = "/tmp/simfs_test_F2.dat"; // File 2

        //------------------------------------------------------------------//
        std::vector<int> payload{};
        std::vector<int> result{};

        //------------------------------------------------------------------//
        size_t const payload_size = 1024*1024*100/sizeof(int); // 100MB payload

};

#endif
