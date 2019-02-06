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

                std::unique_ptr<io::BufferInput<RoutedTime>> input_ptr; 
                std::ofstream ofs;

                // header
                char ident[16] = "SiMFS-Tk";
                char format_version[6] = "6.0";
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
                

                // Byte offsets 
                const size_t T3R_OFFSET_IDENT = 0;
                const size_t T3R_OFFSET_FORMAT_VERSION = 16;
                const size_t T3R_OFFSET_CREATOR_NAME = 22;
                const size_t T3R_OFFSET_CREATOR_VERSION = 40;
                const size_t T3R_OFFSET_FILE_TIME = 52;
                const size_t T3R_OFFSET_CR_LF = 70;
                const size_t T3R_OFFSET_COMMENT = 72;

                const size_t T3R_OFFSET_NUMBER_OF_CHANNELS = 328;
                const size_t T3R_OFFSET_NUMBER_OF_CURVES = 332;
                const size_t T3R_OFFSET_BITS_PER_CHANNEL = 336;
                const size_t T3R_OFFSET_ROUTING_CHANNELS = 340;
                const size_t T3R_OFFSET_NUMBER_OF_BOARDS = 344;
                const size_t T3R_OFFSET_ACTIVE_CURVE = 348;

                const size_t T3R_OFFSET_MEASUREMENT_MODE = 352; 
                const size_t T3R_OFFSET_SUB_MODE = 356;
                const size_t T3R_OFFSET_RANGE_NO = 360;
                const size_t T3R_OFFSET_OFFSET = 364;
                const size_t T3R_OFFSET_ACQUISITION_TIME = 368;
                const size_t T3R_OFFSET_STOP_AT = 372;
                const size_t T3R_OFFSET_STOP_ON_OVFL = 376;
                const size_t T3R_OFFSET_RESTART = 380;
                const size_t T3R_OFFSET_DISPLAY_LIN_LOG = 384;
                const size_t T3R_OFFSET_DISPLAY_TIME_AXIS_FROM = 388;
                const size_t T3R_OFFSET_DISPLAY_TIME_AXIS_TO = 392;
                const size_t T3R_OFFSET_DISPLAY_COUNT_AXIS_FROM = 392;
                const size_t T3R_OFFSET_DISPLAY_COUNT_AXIS_TO = 396;
                const size_t T3R_OFFSET_DISPLAY_CURVE_1_MAP_TO = 400;
                const size_t T3R_OFFSET_DISPLAY_CURVE_1_SHOW = 404;

                const size_t T3R_OFFSET_DISPLAY_CURVE_2_MAP_TO = 404;
                const size_t T3R_OFFSET_DISPLAY_CURVE_2_SHOW = 408;
                const size_t T3R_OFFSET_DISPLAY_CURVE_3_MAP_TO = 412;
                const size_t T3R_OFFSET_DISPLAY_CURVE_3_SHOW = 416;
                const size_t T3R_OFFSET_DISPLAY_CURVE_4_MAP_TO = 420;
                const size_t T3R_OFFSET_DISPLAY_CURVE_4_SHOW = 424;
                const size_t T3R_OFFSET_DISPLAY_CURVE_5_MAP_TO = 428;
                const size_t T3R_OFFSET_DISPLAY_CURVE_5_SHOW = 432;
                const size_t T3R_OFFSET_DISPLAY_CURVE_6_MAP_TO = 436;
                const size_t T3R_OFFSET_DISPLAY_CURVE_6_SHOW = 440;
                const size_t T3R_OFFSET_DISPLAY_CURVE_7_MAP_TO = 444;
                const size_t T3R_OFFSET_DISPLAY_CURVE_7_SHOW = 448;
                const size_t T3R_OFFSET_DISPLAY_CURVE_8_MAP_TO = 452;
                const size_t T3R_OFFSET_DISPLAY_CURVE_8_SHOW = 456;

                const size_t T3R_OFFSET_PARAM_1_START = 460;
                const size_t T3R_OFFSET_PARAM_1_STEP = 464;
                const size_t T3R_OFFSET_PARAM_1_END = 468;
                const size_t T3R_OFFSET_PARAM_2_START = 472;
                const size_t T3R_OFFSET_PARAM_2_STEP = 476;
                const size_t T3R_OFFSET_PARAM_2_END = 480;
                const size_t T3R_OFFSET_PARAM_3_START = 484;
                const size_t T3R_OFFSET_PARAM_3_STEP = 488;
                const size_t T3R_OFFSET_PARAM_3_END = 492;
                const size_t T3R_OFFSET_REPEAT_MODE = 496;
                const size_t T3R_OFFSET_REPEATS_PER_CURVE = 500;
                const size_t T3R_OFFSET_REPEAT_TIME = 504;
                const size_t T3R_OFFSET_REPEAT_WAIT_TIME = 508;

                const size_t T3R_OFFSET_SCRIPT_NAME = 512;
                const size_t T3R_OFFSET_HARDWARE_IDENT = 532;
                const size_t T3R_OFFSET_HARDWARE_VERSION = 548;

                const size_t T3R_OFFSET_BOARD_SERIAL = 556;
                const size_t T3R_OFFSET_CFD_ZERO_CROSS = 560;
                const size_t T3R_OFFSET_CFD_DISCRIMINATION_MIN = 564;
                const size_t T3R_OFFSET_SYNC_LEVEL = 568;
                const size_t T3R_OFFSET_CURVE_OFFSET = 568;
                const size_t T3R_OFFSET_RESOLUTION = 572;
                const size_t T3R_OFFSET_TTTR_GLOBALCLOCK = 576;
                const size_t T3R_OFFSET_EXT_DEVICES = 580;
                const size_t T3R_OFFSET_RESERVERD_001 = 584;
                const size_t T3R_OFFSET_RESERVERD_002 = 588;
                const size_t T3R_OFFSET_RESERVERD_003 = 592;
                const size_t T3R_OFFSET_RESERVERD_004 = 596;
                const size_t T3R_OFFSET_RESERVERD_005 = 600;
                const size_t T3R_OFFSET_SYNC_RATE = 604;
                const size_t T3R_OFFSET_AVERAGE_CFD_RATE = 608;
                const size_t T3R_OFFSET_STOP_AFTER = 612;
                const size_t T3R_OFFSET_STOP_REASON = 616;
                const size_t T3R_OFFSET_NUMBER_OF_RECORDS = 620;
                const size_t T3R_OFFSET_SPEC_HEADER_LENGTH = 624;

        };

    }
}
