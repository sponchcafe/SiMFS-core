#pragma once

#include "component/component.hpp"
#include <thread>
#include <vector>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class T3rWriter : public Component{

            public:

                //-----------------------------------------------------------//
                T3rWriter ();

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_input_id(std::string id);
                void set_sync_rate(double sync);
                void set_output_fname(std::string fn);
                //-----------------------------------------------------------//
               

            private:

                //-----------------------------------------------------------//
                void write_header();
                void write_record(uint32_t record);
                uint32_t pack_record(
                        bool reserved, 
                        bool valid, 
                        unsigned short routing, 
                        unsigned short channel, 
                        unsigned short timetag
                        );

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                std::string input_id = "__input__";
                std::string output_fn = "__input__.t3r";

                std::unique_ptr<io::BufferInput<realtime_t>> input_ptr; 
                std::ofstream ofs;

                // header
                char ident[16] = "SiMFS-Tk";
                char format_version[6] = "";
                char creator_name[18] = "SiMFS-Tk";
                char creator_version[12] = "0.1";
                char file_time[18] = "";
                char cr_lf[2] = {0xA,'\0'};
                char comment[256] = "";
                int32_t number_of_channels = 4096;
                int32_t number_of_curves = 0;
                int32_t bits_per_channel = 0;
                int32_t routing_channels = 1;
                int32_t number_of_boards = 1;
                int32_t active_curve = 0;
                int32_t measurement_mode = 2;
                int32_t sub_mode = 0;
                int32_t range_no = 0;
                int32_t offset = 0;
                int32_t acquisition_time = 0;
                int32_t stop_at = 0;
                int32_t stop_on_ovfl = 0;
                int32_t restart = 0;
                int32_t display_lin_log = 0;
                int32_t display_time_axis_from = 0;
                int32_t display_time_axis_to = 0;
                int32_t display_count_axis_from = 0;
                int32_t display_count_axis_to = 0;
                int32_t display_curve_1_map_to = 0;
                int32_t display_curve_1_show = 0;
                int32_t display_curve_2_map_to = 0;
                int32_t display_curve_2_show = 0;
                int32_t display_curve_3_map_to = 0;
                int32_t display_curve_3_show = 0;
                int32_t display_curve_4_map_to = 0;
                int32_t display_curve_4_show = 0;
                int32_t display_curve_5_map_to = 0;
                int32_t display_curve_5_show = 0;
                int32_t display_curve_6_map_to = 0;
                int32_t display_curve_6_show = 0;
                int32_t display_curve_7_map_to = 0;
                int32_t display_curve_7_show = 0;
                int32_t display_curve_8_map_to = 0;
                int32_t display_curve_8_show = 0;
                float param_1_start = 0;
                float param_1_step = 0;
                float param_1_end = 0;
                float param_2_start = 0;
                float param_2_step = 0;
                float param_2_end = 0;
                float param_3_start = 0;
                float param_3_step = 0;
                float param_3_end = 0;
                int32_t repeat_mode = 0;
                int32_t repeats_per_curve = 0;
                int32_t repeat_time = 0;
                int32_t repeat_wait_time = 0;
                char script_name[20] = "";
                char hardware_ident[16] = "SiMFS-Tk";
                char hardware_version[8] = "0.1";
                int32_t board_serial = 0;
                int32_t cfd_zero_cross = 0;
                int32_t cfd_discrimination_min = 0;
                int32_t sync_level = 0;
                int32_t curve_offset = 0;
                int32_t resolution = 0;
                int32_t tttr_globalclock = 0;
                int32_t ext_devices = 0;
                int32_t reserverd_001 = 0;
                int32_t reserverd_002 = 0;
                int32_t reserverd_003 = 0;
                int32_t reserverd_004 = 0;
                int32_t reserverd_005 = 0;
                int32_t sync_rate = 0;
                int32_t average_cfd_rate = 0;
                int32_t stop_after = 0;
                int32_t stop_reason = 0;
                int32_t number_of_records = 0;
                int32_t spec_header_length = 8;
                int32_t reserved_006 = 0;

        };

    }
}
