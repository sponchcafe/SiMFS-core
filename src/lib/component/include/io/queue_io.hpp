#ifndef SIMFS_QUEUE_IO_H
#define SIMFS_QUEUE_IO_H

#include "queue_fs.hpp"
#include "io/base_io.hpp"

namespace sim{
    namespace queue_io{

        //-----------------------------------------------------------------------//
        // Time an input thread waits on an empty queue until it rechecks if the 
        // producer thread is done (eof == true)
        //-----------------------------------------------------------------------//
        constexpr size_t DEAD_TIME_MILLIS = 10;

        //-----------------------------------------------------------------------//
        // Template for lock-free queue based input.
        //-----------------------------------------------------------------------//
        template <typename T> class QueueInput : public Input<T>{

            public:

                //---------------------------------------------------------------//
                QueueInput<T>(std::string id) : 
                    Input<T>(id),
                    queue_handle(open<T>(id)) {
                        eof = !get_next();
                    }

                //-Copy-ctor:-DELETED--------------------------------------------//
                QueueInput<T> (QueueInput<T> &other) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                QueueInput<T> &operator=(QueueInput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                QueueInput<T>(QueueInput<T> &&other) = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                QueueInput<T> &operator=(QueueInput<T> &&other) = default;

                //---------------------------------------------------------------//
                bool get(T &target) override {
                    target = current;
                    if (!eof && !get_next()){
                        eof = true;
                        return true;
                    }
                    return !eof;
                }

                //---------------------------------------------------------------//
                T peek() const override {
                    return current;
                }

            private:

                //---------------------------------------------------------------//
                bool get_next(){
                    T next = current;
                    do {
                        if (queue_handle.queue->wait_dequeue_timed(next, 
                                    std::chrono::milliseconds(DEAD_TIME_MILLIS))){
                            current = next;
                            return true;
                        } 
                    }while(!queue_handle.eof->load());
                    return false;
                }

                //---------------------------------------------------------------//
                queue_handle_t<T> &queue_handle;
                bool eof = false;
                T current;

        };

        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        
        //-----------------------------------------------------------------------//
        // Template for lock-free queue based output.
        //-----------------------------------------------------------------------//
        template <typename T> class QueueOutput : public Output<T>{

            public:

                //---------------------------------------------------------------//
                QueueOutput<T>(std::string id) :
                    Output<T>(id),
                    queue_handle(open<T>(id)) { }

                //---------------------------------------------------------------//
                ~QueueOutput<T>(){
                    // notify the user that the output is done.
                    queue_handle.eof->store(true);
                }

                //-Copy-ctor:-DELETED--------------------------------------------//
                QueueOutput<T>(QueueOutput<T> &source) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                QueueOutput<T> &operator=(QueueOutput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                QueueOutput<T>(QueueOutput<T> &&other)  = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                QueueOutput<T> &operator=(QueueOutput<T> &&other) = default;

                //---------------------------------------------------------------//
                void put(T &item) override {
                    queue_handle.queue->enqueue(item);
                }

            private:

                //---------------------------------------------------------------//
                queue_handle_t<T> &queue_handle;

        };

    }

}

#endif
