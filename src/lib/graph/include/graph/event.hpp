#ifndef SIM_RATE_GRAPH_EVENT_H
#define SIM_RATE_GRAPH_EVENT_H

#include <string>
#include <iostream>
#include "graph/event.hpp"
#include "graph/rate_graph.hpp"

namespace sim{
    namespace graph{
        
        class Event{

            public:
                
                //ctor, all const and non-const move and copy semantics...
                Event(Action *target, const double time);
                Event(Event &other) : Event(other.target, other.time){}
                Event(const Event &other) : Event(other.target, other.time){}
                Event(Event &&other) : Event(other.target, other.time){}
                Event(const Event &&other) : Event(other.target, other.time){}
                Event &operator=(Event &other) {
                    Event *e = new Event(other.target, other.time);
                    other.target = nullptr;
                    other.time = -1.0;
                    return *e;
                }
                Event &operator=(const Event &other) {
                    Event *e = new Event(other.target, other.time);
                    return *e;
                }
                Event &operator=(Event &&other) {
                    this->time = other.time;
                    this->target = other.target;
                    return *this; 
                }
               
                Event &operator=(const Event &&other) {
                    this->time = other.time;
                    this->target = other.target;
                    return *this;
                }
                ~Event() = default;

                Action *target = nullptr;
                double time = 0.0;
                bool operator<(const Event &other) const;

        };

        std::ostream &operator<<(std::ostream &os, const Event &event);
    }
}

#endif
