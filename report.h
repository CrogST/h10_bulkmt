#ifndef REPORT_H
#define REPORT_H

#include <list>
#include "out_base.h"

class report {
    std::list<out_base *> subs;
public:
    void subscribe(out_base * ptr);
    void notify_all(cmd_list_t val, time_point_t time);
    void drop_all();
};

#endif // REPORT_H
