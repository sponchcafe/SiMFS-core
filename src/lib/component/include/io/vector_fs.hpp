#ifndef SIMFS_VECTOR_FS_H
#define SIMFS_VECTOR_FS_H

#include <vector>
#include <unordered_map>

namespace sim{
    namespace vector_io{

        //-------------------------------------------------------------------//
        //-Type-Definitions--------------------------------------------------//
        
        //-------------------------------------------------------------------//
        using key_t = std::string;
        template <typename T> using val_t = std::unique_ptr<std::vector<T>>;
        template <typename T> using item_t = std::pair<key_t, val_t<T>>;

        //-------------------------------------------------------------------//
        //-------------------------------------------------------------------//

        //-------------------------------------------------------------------//

        template <typename T> std::unordered_map<key_t, val_t<T>> fs = 
            std::unordered_map<key_t, val_t<T>>();

        //-------------------------------------------------------------------//
        template <typename T> std::vector<T> &open(std::string id){
            try {fs<T>.at(id);}
            catch (std::out_of_range &e){
                fs<T>.emplace(item_t<T>(id, val_t<T>(
                                new std::vector<T>()
                                )));
            }
            val_t<T> &vec = fs<T>.at(id);
            return *vec.get();
        }

        //-------------------------------------------------------------------//
        
    }
}

#endif
