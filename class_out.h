#pragma once

#include <iostream>
#include <list>

using cmd_list_t = std::list<std::string>;

class base {
public:
    virtual ~base() = default;
    virtual void write(std::string str) = 0;
};

class report;

class log_out : base {
public:
    log_out(report * rp);
    void write(std::string str) override {
        std::cout << "log: " << __FUNCTION__ << ": " << str << std::endl;
    }
};

class write_out : base {
public:
    write_out(report * rp);
    void write(std::string str) override {
        std::cout << "file: " << __FUNCTION__ << ": " << str << std::endl;
    }
};

class report {
    std::list<base *> subs;
public:
    void subscribe(base * ptr) {
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
