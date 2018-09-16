#include "graph/event.hpp"

namespace sim{
    namespace graph{

        Event::Event(Action *target, double time) : target(target), time(time){}

        bool Event::operator<(const Event &other) const {
            return time > other.time;
        }
        
        std::ostream &operator<<(std::ostream &os, const Event &event){
            os << "<Event at " << &event << ", time=" << event.time << "s>";
            if (event.target != nullptr){
                os << " => ";
                event.target->print_info(os);
            }else{
                os << " => <none>" << std::endl;
            }
            return os;
        }

    }
}

