#include "report.h"

void report::subscribe(out_base * ptr) {
    subs.push_back(ptr);
}

void report::notify_all(cmd_list_t val, time_point_t time) {
    for(auto & el : subs) {
        el->signal(val, time);
    }
}

void report::drop_all() {
    for(auto & el : subs) {
        el->drop();
    }
}
