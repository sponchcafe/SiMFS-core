#include "t3r_writer/component.hpp"
#include "io/buffer.hpp"
#include <iterator>
#include <fstream>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        T3rWriter::T3rWriter(){ }

        //-------------------------------------------------------------------//
        void T3rWriter::set_input_id(std::string id){
            input_id = id;
        }
        void T3rWriter::set_output_fname(std::string fn){
            output_fn = fn;
        }
        void T3rWriter::set_sync_rate(double rate){
            sync_rate = rate;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void T3rWriter::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_input_id(params.at("input"));
            set_output_fname(params.at("output_file"));
            set_sync_rate(params.at("SyncRate"));

        }

        //-------------------------------------------------------------------//
        json T3rWriter::get_json(){

            json j;

            j["output_file"] = output_fn;
            j["input"] = input_id;
            j["SyncRate"] = sync_rate;


            return j;

        }


        //-------------------------------------------------------------------//
        void T3rWriter::init() {
            input_ptr = std::make_unique<io::BufferInput<realtime_t>> (input_id);
            ofs = std::ofstream{output_fn, std::ios::binary};
        }

        //-------------------------------------------------------------------//
        void T3rWriter::write_header() {
            
            ofs.write(reinterpret_cast<char *>(&ident), sizeof(ident));
            ofs.write(reinterpret_cast<char *>(&format_version), sizeof(format_version));
            ofs.write(reinterpret_cast<char *>(&creator_name), sizeof(creator_name));
            ofs.write(reinterpret_cast<char *>(&creator_version), sizeof(creator_version));
            ofs.write(reinterpret_cast<char *>(&file_time), sizeof(file_time));
            ofs.write(reinterpret_cast<char *>(&cr_lf), sizeof(cr_lf));
            ofs.write(reinterpret_cast<char *>(&comment), sizeof(comment));
            ofs.write(reinterpret_cast<char *>(&number_of_channels), sizeof(number_of_channels));
            ofs.write(reinterpret_cast<char *>(&number_of_curves), sizeof(number_of_curves));
            ofs.write(reinterpret_cast<char *>(&bits_per_channel), sizeof(bits_per_channel));
            ofs.write(reinterpret_cast<char *>(&routing_channels), sizeof(routing_channels));
            ofs.write(reinterpret_cast<char *>(&number_of_boards), sizeof(number_of_boards));
            ofs.write(reinterpret_cast<char *>(&active_curve), sizeof(active_curve));
            ofs.write(reinterpret_cast<char *>(&measurement_mode), sizeof(measurement_mode));
            ofs.write(reinterpret_cast<char *>(&sub_mode), sizeof(sub_mode));
            ofs.write(reinterpret_cast<char *>(&range_no), sizeof(range_no));
            ofs.write(reinterpret_cast<char *>(&offset), sizeof(offset));
            ofs.write(reinterpret_cast<char *>(&acquisition_time), sizeof(acquisition_time));
            ofs.write(reinterpret_cast<char *>(&stop_at), sizeof(stop_at));
            ofs.write(reinterpret_cast<char *>(&stop_on_ovfl), sizeof(stop_on_ovfl));
            ofs.write(reinterpret_cast<char *>(&restart), sizeof(restart));
            ofs.write(reinterpret_cast<char *>(&display_lin_log), sizeof(display_lin_log));
            ofs.write(reinterpret_cast<char *>(&display_time_axis_from), sizeof(display_time_axis_from));
            ofs.write(reinterpret_cast<char *>(&display_time_axis_to), sizeof(display_time_axis_to));
            ofs.write(reinterpret_cast<char *>(&display_count_axis_from), sizeof(display_count_axis_from));
            ofs.write(reinterpret_cast<char *>(&display_count_axis_to), sizeof(display_count_axis_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_1_map_to), sizeof(display_curve_1_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_1_show), sizeof(display_curve_1_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_2_map_to), sizeof(display_curve_2_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_2_show), sizeof(display_curve_2_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_3_map_to), sizeof(display_curve_3_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_3_show), sizeof(display_curve_3_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_4_map_to), sizeof(display_curve_4_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_4_show), sizeof(display_curve_4_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_5_map_to), sizeof(display_curve_5_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_5_show), sizeof(display_curve_5_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_6_map_to), sizeof(display_curve_6_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_6_show), sizeof(display_curve_6_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_7_map_to), sizeof(display_curve_7_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_7_show), sizeof(display_curve_7_show));
            ofs.write(reinterpret_cast<char *>(&display_curve_8_map_to), sizeof(display_curve_8_map_to));
            ofs.write(reinterpret_cast<char *>(&display_curve_8_show), sizeof(display_curve_8_show));
            ofs.write(reinterpret_cast<char *>(&param_1_start), sizeof(param_1_start));
            ofs.write(reinterpret_cast<char *>(&param_1_step), sizeof(param_1_step));
            ofs.write(reinterpret_cast<char *>(&param_1_end), sizeof(param_1_end));
            ofs.write(reinterpret_cast<char *>(&param_2_start), sizeof(param_2_start));
            ofs.write(reinterpret_cast<char *>(&param_2_step), sizeof(param_2_step));
            ofs.write(reinterpret_cast<char *>(&param_2_end), sizeof(param_2_end));
            ofs.write(reinterpret_cast<char *>(&param_3_start), sizeof(param_3_start));
            ofs.write(reinterpret_cast<char *>(&param_3_step), sizeof(param_3_step));
            ofs.write(reinterpret_cast<char *>(&param_3_end), sizeof(param_3_end));
            ofs.write(reinterpret_cast<char *>(&repeat_mode), sizeof(repeat_mode));
            ofs.write(reinterpret_cast<char *>(&repeats_per_curve), sizeof(repeats_per_curve));
            ofs.write(reinterpret_cast<char *>(&repeat_time), sizeof(repeat_time));
            ofs.write(reinterpret_cast<char *>(&repeat_wait_time), sizeof(repeat_wait_time));
            ofs.write(reinterpret_cast<char *>(&script_name), sizeof(script_name));
            ofs.write(reinterpret_cast<char *>(&hardware_ident), sizeof(hardware_ident));
            ofs.write(reinterpret_cast<char *>(&hardware_version), sizeof(hardware_version));
            ofs.write(reinterpret_cast<char *>(&board_serial), sizeof(board_serial));
            ofs.write(reinterpret_cast<char *>(&cfd_zero_cross), sizeof(cfd_zero_cross));
            ofs.write(reinterpret_cast<char *>(&cfd_discrimination_min), sizeof(cfd_discrimination_min));
            ofs.write(reinterpret_cast<char *>(&sync_level), sizeof(sync_level));
            ofs.write(reinterpret_cast<char *>(&curve_offset), sizeof(curve_offset));
            ofs.write(reinterpret_cast<char *>(&resolution), sizeof(resolution));
            ofs.write(reinterpret_cast<char *>(&tttr_globalclock), sizeof(tttr_globalclock));
            ofs.write(reinterpret_cast<char *>(&ext_devices), sizeof(ext_devices));
            ofs.write(reinterpret_cast<char *>(&reserverd_001), sizeof(reserverd_001));
            ofs.write(reinterpret_cast<char *>(&reserverd_002), sizeof(reserverd_002));
            ofs.write(reinterpret_cast<char *>(&reserverd_003), sizeof(reserverd_003));
            ofs.write(reinterpret_cast<char *>(&reserverd_004), sizeof(reserverd_004));
            ofs.write(reinterpret_cast<char *>(&reserverd_005), sizeof(reserverd_005));
            ofs.write(reinterpret_cast<char *>(&sync_rate), sizeof(sync_rate));
            ofs.write(reinterpret_cast<char *>(&average_cfd_rate), sizeof(average_cfd_rate));
            ofs.write(reinterpret_cast<char *>(&stop_after), sizeof(stop_after));
            ofs.write(reinterpret_cast<char *>(&stop_reason), sizeof(stop_reason));
            ofs.write(reinterpret_cast<char *>(&number_of_records), sizeof(number_of_records));
            ofs.write(reinterpret_cast<char *>(&spec_header_length), sizeof(spec_header_length));
            ofs.write(reinterpret_cast<char *>(&reserved_006), sizeof(reserved_006));

        }

        //-------------------------------------------------------------------//
        void T3rWriter::write_record(uint32_t record){
            ofs.write(reinterpret_cast<char *>(&record), sizeof(record));
        }

        //-------------------------------------------------------------------//
        uint32_t T3rWriter::pack_record(
            bool reserved, 
            bool valid, 
            unsigned short routing, 
            unsigned short channel, 
            unsigned short timetag) {

            uint32_t res = (reserved << 31) & 0x80000000;
            uint32_t val = (valid << 30) &    0x40000000;
            uint32_t rot = (routing << 28) &  0x30000000;
            uint32_t chn = (channel << 16) &  0x0fff0000;
            uint32_t tag = timetag         &  0x0000ffff;

            return res | val | rot | chn | tag;

        }

        //-------------------------------------------------------------------//
        void T3rWriter::run(){

            realtime_t tag;
            uint64_t ticks;
            uint32_t ovfls = 0;
            uint32_t ovfls_written = 0;
            uint16_t t3r_tag = 0;

            int count = 0;

            if(ofs.is_open()){

                write_header();

                while(input_ptr->get(tag)){

                    ticks   = (uint64_t)  (tag * sync_rate);
                    ovfls   = (uint32_t) ((ticks & 0xffffffff0000) >> 16);
                    t3r_tag = (uint16_t)  (ticks & 0x0000ffff);

                    while(ovfls_written < ovfls){
                        write_record(pack_record(false, false, 0u, 2048u, 0u));
                        ovfls_written++;
                    }

                    write_record(pack_record(false, true, 0u, 0u, t3r_tag));
                    count++;
                
                }

            }

        }

    }

}
