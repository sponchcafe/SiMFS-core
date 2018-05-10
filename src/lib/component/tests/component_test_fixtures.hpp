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
        template <
            template <class InputT> class InputT,
            template <class Output> class OutputT
        > void serial_PIC_test(){

            {
                ProducerComponent<OutputT> prod{};
                prod.set_data(payload);
                prod.set_output(f1);
                prod.run();
            }
            {
                TransformComponent<InputT, OutputT> trans{};
                trans.set_input(f1);
                trans.set_output(f2);
                trans.run();
            }
            {
                ConsumerComponent<InputT> cons{};
                cons.set_input(f2);
                cons.run();
                result = cons.get_data();
            }
        }

        //------------------------------------------------------------------//
        template <
            template <class InputT> class InputT,
            template <class Output> class OutputT
        > void parallel_PIC_test(){

            std::thread prod_thread = std::thread ( [&]() {
                    ProducerComponent<OutputT> prod{};
                    prod.set_data(payload);
                    prod.set_output(p1);
                    prod.run();
                    });

            std::thread trans_thread = std::thread ( [&]() {
                    TransformComponent<InputT, OutputT> trans{};
                    trans.set_input(p1);
                    trans.set_output(p2);
                    trans.run();
                    });

            std::thread cons_thread = std::thread ( [&]() {
                    ConsumerComponent<InputT> cons{};
                    cons.set_input(p2);
                    cons.run();
                    result = cons.get_data();
                    });

            prod_thread.join();
            trans_thread.join();
            cons_thread.join();

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
        size_t const payload_size = 1024*1024*200/sizeof(int); // 200MB payload


};

#endif
