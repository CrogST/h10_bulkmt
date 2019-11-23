#pragma once

#include <iostream>
#include <list>

class out_base {
public:
    virtual ~out_base() = default;
    virtual void write(std::string str) = 0;
};

class report;

class log_out : out_base {
public:
    log_out(report * rp);
    void write(std::string str) override {
        std::cout << "log: " << __FUNCTION__ << ": " << str << std::endl;
    }
};

class write_out : out_base {
public:
    write_out(report * rp);
    void write(std::string str) override {
        std::cout << "file: " << __FUNCTION__ << ": " << str << std::endl;
    }
};

class report {
    std::list<out_base *> subs;
public:
    void subscribe(out_base * ptr) {
        subs.push_back(ptr);
    }
    void notify_all(std::string str) {
        for(auto & el : subs) {
            el->write(str);
        }
    }
};

log_out::log_out(report * rp) {
    rp->subscribe(this);
}

write_out::write_out(report * rp) {
    rp->subscribe(this);
}
