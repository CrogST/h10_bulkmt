#pragma once

#include <iostream>
#include <fstream>

#include "user_types.h"

class out_base {
public:
    virtual ~out_base() = default;
    virtual void write(cmd_list_t str, time_point_t tp) = 0;
};

class report;

class log_out : out_base {
public:
    log_out(report * rp);
    void write(cmd_list_t val, time_point_t) override {
        std::cout << "bulk:";
        for(auto el = val.begin(); el != val.end(); el++) {
            if(el != val.begin()) std::cout << ",";
            std::cout << " " << *el;
        }
        std::cout << std::endl;
    }
};

class write_out : out_base {
public:
    write_out(report * rp);
    void write(cmd_list_t val, time_point_t tp) override {

        auto cnt = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
        auto name = std::to_string(cnt.count());
        std::ofstream myfile;
        myfile.open ("bulk" + name + ".log");
        for(const auto & el : val) {
            myfile << el << std::endl;
        }
        myfile.close();
    }
};

class report {
    std::list<out_base *> subs;
public:
    void subscribe(out_base * ptr) {
        subs.push_back(ptr);
    }
    void notify_all(cmd_list_t val, time_point_t time) {
        for(auto & el : subs) {
            el->write(val, time);
        }
    }
};

log_out::log_out(report * rp) {
    rp->subscribe(this);
}

write_out::write_out(report * rp) {
    rp->subscribe(this);
}
