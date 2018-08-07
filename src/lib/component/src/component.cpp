#include "component/component.hpp"

using json = nlohmann::json;

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        bool filter_json(json const &j, json_filter_t filter){
            return filter(j);
        }

        //-------------------------------------------------------------------//
        bool filter_json_all(json const &j, std::initializer_list<json_filter_t> filters){
            for (auto f: filters) if (!f(j)) return false;
            return true;
        }

        //-------------------------------------------------------------------//
        bool filter_json_any(json const &j, std::initializer_list<json_filter_t> filters){
            for (auto f: filters) if (f(j)) return true;
            return false;
        }

    }
}
