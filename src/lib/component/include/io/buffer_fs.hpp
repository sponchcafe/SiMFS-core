#pragma once

#include <unordered_map>
#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <iostream>

#include "queue/readerwriterqueue.h"
#include "queue/atomicops.h"


using namespace moodycamel;

namespace sim{
    namespace io{
        
        //-----------------------------------------------------------------------//
        //-Type-Definitions------------------------------------------------------//
        
        constexpr size_t DEFAULT_QUEUE_SIZE = 64;

        //-----------------------------------------------------------------------//
        using key_t = std::string;
        template <typename T> using queue_t = BlockingReaderWriterQueue<T>;

        //-----------------------------------------------------------------------//
        template <typename T> 
            struct queue_handle_t{ 
                std::unique_ptr<queue_t<T>> queue;
                std::unique_ptr<std::atomic<size_t>> size;
            };

        //-----------------------------------------------------------------------//
        template <typename T> using queue_file_t = 
            std::pair<key_t, queue_handle_t<T>>;

        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        
        
        //-----------------------------------------------------------------------//
        // The unordered map serves as a named store for queue objects in analogy
        // to named pipes in a file system.
        //-----------------------------------------------------------------------//
        template <typename T> 
            std::unordered_map<key_t, queue_handle_t<T>> fs = 
            std::unordered_map<key_t, queue_handle_t<T>>();

        //-----------------------------------------------------------------------//
        // Mutex for thread safe access to the pseudo-filesystem
        //-----------------------------------------------------------------------//
        template <typename T> std::mutex mtx{}; // One mutex per data type T.

        //-----------------------------------------------------------------------//
        // Assembly of a new queue object in the pseudo-filesystem.
        //-----------------------------------------------------------------------//
        template <typename T> void create_new(std::string id){
            queue_file_t<T> q_file{};
            q_file.first = id;
            auto insert = fs<T>.insert(std::move(q_file)).first;
            insert->second.queue.reset(new queue_t<T>());
            insert->second.size.reset(new std::atomic<size_t>(0));
        }

        //-----------------------------------------------------------------------//
        // Thread safe access of queue objects. Mutex protected access to the
        // hashmap pseudo-filesystem of queues.
        //-----------------------------------------------------------------------//
        template <typename T> queue_handle_t<T> &open(std::string id){

            //-------------------------------------------------------------------//
            mtx<T>.lock(); // Critical section begin ----------------------------//
            //-------------------------------------------------------------------//

            try { fs<T>.at(id); }
            catch (std::out_of_range &e){ 
                create_new<T>(id);
            }
            queue_handle_t<T> &queue = fs<T>.at(id);
                    
            //-------------------------------------------------------------------//
            mtx<T>.unlock(); // Critical section end ----------------------------//
            //-------------------------------------------------------------------//

            return queue;

        }

        //-----------------------------------------------------------------------//
        template <typename T> void close(std::string const id){

            //-------------------------------------------------------------------//
            mtx<T>.lock(); // Critical section begin ----------------------------//
            //-------------------------------------------------------------------//
            
            fs<T>.erase(id);

            //-------------------------------------------------------------------//
            mtx<T>.unlock(); // Critical section end ----------------------------//
            //-------------------------------------------------------------------//
        }

        //-----------------------------------------------------------------------//
        template <typename T> void clear_buffer_fs(){

            //-------------------------------------------------------------------//
            mtx<T>.lock(); // Critical section begin ----------------------------//
            //-------------------------------------------------------------------//
            
            fs<std::vector<T>>.clear();

            //-------------------------------------------------------------------//
            mtx<T>.unlock(); // Critical section end ----------------------------//
            //-------------------------------------------------------------------//
            
        }

        //-----------------------------------------------------------------------//
        
    }
}
