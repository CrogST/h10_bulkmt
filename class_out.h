#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <list>

#include "user_types.h"

class out_base {
public:
    virtual ~out_base() = default;
    virtual void write(cmd_list_t str) = 0;
};

class report;

class log_out : out_base {
public:
    log_out(report * rp);
    void write(cmd_list_t val) override {
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
    void write(cmd_list_t val) override {

        auto now = std::chrono::system_clock::now();
        auto cnt = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        auto name = std::to_string(cnt.count());
        std::ofstream myfile;
        myfile.open (name);
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
    void notify_all(cmd_list_t val) {
        for(auto & el : subs) {
            el->write(val);
        }
    }
};

log_out::log_out(report * rp) {
    rp->subscribe(this);
}

write_out::write_out(report * rp) {
    rp->subscribe(this);
}
