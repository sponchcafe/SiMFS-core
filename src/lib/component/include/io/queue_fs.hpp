#ifndef SIMFS_QUEUE_FS_T
#define SIMFS_QUEUE_FS_T

#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

#include "queue/readerwriterqueue.h"
#include "queue/atomicops.h"


using namespace moodycamel;

namespace sim{
    namespace queue_io{
        
        //-----------------------------------------------------------------------//
        //-Type-Definitions------------------------------------------------------//
        
        constexpr size_t DEFAULT_QUEUE_SIZE = 1024*1024;

        //-----------------------------------------------------------------------//
        using key_t = std::string;
        template <typename T> using queue_t = BlockingReaderWriterQueue<T>;
        template <typename T> using queue_ptr_t = std::unique_ptr<queue_t<T>>;
        using atomic_ptr_t = std::unique_ptr<std::atomic<bool>>;

        //-----------------------------------------------------------------------//
        // The atomic eof bool indicates the completion of the producer thread's
        // task. The writer end is considered closed if eof==true;
        //-----------------------------------------------------------------------//
        template <typename T> 
            struct queue_handle_t{ 
                queue_ptr_t<T> queue; 
                atomic_ptr_t eof;
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
        std::mutex mtx{};

        //-----------------------------------------------------------------------//
        // Assembly of a new queue object in the pseudo-filesystem.
        //-----------------------------------------------------------------------//
        template <typename T> void create_new(std::string id){
            queue_file_t<T> q_file{};
            q_file.first = id;
            auto insert = fs<T>.insert(std::move(q_file)).first;
            insert->second.queue.reset(new queue_t<T>());
            insert->second.eof.reset(new std::atomic<bool>(false));
        }

        //-----------------------------------------------------------------------//
        // Thread safe access of queue objects. Mutex protected access to the
        // hashmap pseudo-filesystem of queues.
        //-----------------------------------------------------------------------//
        template <typename T> queue_handle_t<T> &open(std::string id){

            //-------------------------------------------------------------------//
            mtx.lock(); // Critical section begin -------------------------------//
            //-------------------------------------------------------------------//

            try { fs<T>.at(id); }
            catch (std::out_of_range &e){ create_new<T>(id); }
            queue_handle_t<T> &queue = fs<T>.at(id);
                    
            //-------------------------------------------------------------------//
            mtx.unlock(); // Critical section end -------------------------------//
            //-------------------------------------------------------------------//

            return queue;

        }

        //-----------------------------------------------------------------------//
        
    }
}

#endif