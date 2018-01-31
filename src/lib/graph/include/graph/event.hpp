#ifndef SIM_GRAPH_EVENT_H
#define SIM_GRAPH_EVENT_H

#include "graph/action.hpp"

#include <string>
#include <iostream>

namespace sim{
    namespace graph{
        
        class Event{

            public:

                //-----------------------------------------------------------//
                Event(Action *target, double time);


                //-----------------------------------------------------------//
                Event(Event &other) : Event(other.target, other.time){}


                //-----------------------------------------------------------//
                Event(Event const &other) : Event(other.target, other.time){}


                //-----------------------------------------------------------//
                Event(Event &&other) : Event(other.target, other.time){}


                //-----------------------------------------------------------//
                Event(const Event &&other) : Event(other.target, other.time){}


                //-----------------------------------------------------------//
                Event &operator=(Event &other) {
                    Event *e = new Event(other.target, other.time);
                    other.target = nullptr;
                    other.time = -1.0;
                    return *e;
                }


                //-----------------------------------------------------------//
                Event &operator=(const Event &other) {
                    Event *e = new Event(other.target, other.time);
                    return *e;
                }


                //-----------------------------------------------------------//
                Event &operator=(Event &&other) {
                    this->time = other.time;
                    this->target = other.target;
                    return *this; 
                }
               

                //-----------------------------------------------------------//
                Event &operator=(const Event &&other) {
                    this->time = other.time;
                    this->target = other.target;
                    return *this;
                }


                //-----------------------------------------------------------//
                ~Event() = default;


                //-----------------------------------------------------------//
                Action *target = nullptr;
                double time = 0.0;
                bool operator<(Event const &other) const;

        };

        std::ostream &operator<<(std::ostream &os, Event const &event);
    }
}

#endif
