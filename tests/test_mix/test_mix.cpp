#include "catch.hpp"
#include "mixer/component.hpp"
#include "io/buffer.hpp"
#include <string>


using namespace sim;
using namespace sim::comp;
using namespace sim::io;


namespace sim{

    bool operator!= (RoutedTime const &lhs, RoutedTime const &rhs){
        return lhs.route != rhs.route || lhs.time != rhs.time;
    }

    std::ostream &operator<< (std::ostream &os, RoutedTime const &rt){
        return os << "{" << rt.route << ", " << rt.time << "}";
    }

}


/*---------------------------------------------------------------------------*/
TEST_CASE("Sorting test", "[mix][sanity]"){

    GIVEN("Two vectors of interleaved sorted photons"){

        std::vector<realtime_t> tags_1{};
        std::vector<realtime_t> tags_2{};
        std::vector<realtime_t> sorted{};
        std::vector<RoutedTime> sorted_routed{};

        for (int i=0; i<100; i++){
            double t1 = 0.001*i;
            double t2 = t1+0.0005;

            tags_1.push_back(t1);
            tags_2.push_back(t2);
            sorted.push_back(t1);
            sorted.push_back(t2);
            sorted_routed.push_back({t1, 0});
            sorted_routed.push_back({t2, 1});
        }

        std::string out_id{"mix_out"};

        WHEN("It is passed as single input to a mixer instance"){

            std::vector<std::string> in_ids{"tags_1"};
            std::vector<realtime_t> result{};

            Mixer mix{};
            mix.set_routed(false);
            mix.set_photon_output_id(out_id);
            mix.set_photon_input_ids(in_ids);

            auto mix_thr = run_component<Mixer>(mix, true);
            auto in_thr = vector2buffer_thread<realtime_t>(tags_1, in_ids[0]);
            auto out_thr = buffer2vector_thread<realtime_t>(out_id, result);

            in_thr.join();
            mix_thr.join();
            out_thr.join();

            THEN("After thread termination, all data is passed to the output"){
                REQUIRE_THAT(result, Catch::Matchers::Equals(tags_1));
            }

        }

        WHEN("Both vectors are passed as input"){

            std::vector<std::string> in_ids{"tags_1", "tags_2"};
            std::vector<realtime_t> result{};

            Mixer mix{};
            mix.set_routed(false);
            mix.set_photon_output_id(out_id);
            mix.set_photon_input_ids(in_ids);

            auto mix_thr = run_component<Mixer>(mix, true);
            auto out_thr = buffer2vector_thread<realtime_t>(out_id, result);
            auto in_thr_1 = vector2buffer_thread<realtime_t>(tags_1, in_ids[0]);
            auto in_thr_2 = vector2buffer_thread<realtime_t>(tags_2, in_ids[1]);

            in_thr_1.join();
            in_thr_2.join();
            mix_thr.join();
            out_thr.join();

            THEN("After thread termination, all data is passed to the output"){
                REQUIRE_THAT(result, Catch::Matchers::Equals(sorted));
            }

        }

        WHEN("Both vectors are passed as input with routing"){

            std::vector<std::string> in_ids{"tags_1", "tags_2"};
            std::vector<RoutedTime> result{};

            Mixer mix{};
            mix.set_routed(true);
            mix.set_photon_output_id(out_id);
            mix.set_photon_input_ids(in_ids);

            auto mix_thr = run_component<Mixer>(mix, true);
            auto out_thr = buffer2vector_thread<RoutedTime>(out_id, result);
            auto in_thr_1 = vector2buffer_thread<realtime_t>(tags_1, in_ids[0]);
            auto in_thr_2 = vector2buffer_thread<realtime_t>(tags_2, in_ids[1]);

            in_thr_1.join();
            in_thr_2.join();
            mix_thr.join();
            out_thr.join();

            THEN("After thread termination, all data is passed to the output"){
                REQUIRE_THAT(result, Catch::Matchers::Equals(sorted_routed));
            }

        }

    }

}

/*---------------------------------------------------------------------------*/
TEST_CASE("Heartbeat test", "[heartbeat]"){

    GIVEN("A vector of tags with negative heartbeats"){

        std::vector<realtime_t> tags_1{};
        std::vector<realtime_t> sorted{};

        int beat_freq = 10;
        for (int i=0; i<100; i++){
            double t1 = 0.001*i;
            if (i%beat_freq != 0){
                tags_1.push_back(t1);
                sorted.push_back(t1);
            } else{
                t1*=-1;
                tags_1.push_back(t1);
            }
        }

        std::string out_id{"mix_out"};

        WHEN("It is passed as single input to a mixer instance without heartbeat"){

            std::vector<std::string> in_ids{"tags_1"};
            std::vector<realtime_t> result{};

            Mixer mix{};
            mix.set_routed(false);
            mix.set_heartbeat(false);
            mix.set_photon_output_id(out_id);
            mix.set_photon_input_ids(in_ids);

            auto mix_thr = run_component<Mixer>(mix, true);
            auto in_thr = vector2buffer_thread<realtime_t>(tags_1, in_ids[0]);
            auto out_thr = buffer2vector_thread<realtime_t>(out_id, result);

            in_thr.join();
            mix_thr.join();
            out_thr.join();

            THEN("After thread termination, only the real tags are passed"){
                REQUIRE_THAT(result, Catch::Matchers::Equals(sorted));
            }

        }

        WHEN("It is passed as single input to a mixer instance with heartbeat"){

            std::vector<std::string> in_ids{"tags_1"};
            std::vector<realtime_t> result{};

            Mixer mix{};
            mix.set_routed(false);
            mix.set_heartbeat(true);
            mix.set_photon_output_id(out_id);
            mix.set_photon_input_ids(in_ids);

            auto mix_thr = run_component<Mixer>(mix, true);
            auto in_thr = vector2buffer_thread<realtime_t>(tags_1, in_ids[0]);
            auto out_thr = buffer2vector_thread<realtime_t>(out_id, result);

            in_thr.join();
            mix_thr.join();
            out_thr.join();

            THEN("After thread termination, all tags are passed"){
                REQUIRE_THAT(result, Catch::Matchers::Equals(tags_1));
            }

        }
    }
}
