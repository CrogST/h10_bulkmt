#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <sstream>
#include <condition_variable>

#include "user_types.h"

const bool test_log = true;
#define _log(x) if(test_log) std::cout << x

class out_base {
protected:
    cmd_list_t ls;
    time_point_t tp;
    std::mutex mx;
    std::condition_variable cv;
    bool sleep = true;
    virtual void write() = 0;
    bool q = false;
public:
    virtual ~out_base() = default;
    void signal(cmd_list_t s, time_point_t t) {
        ls = s;
        tp = t;
        sleep = false;
        cv.notify_one();
    }
    void thread_exec() {
        std::unique_lock<std::mutex> lk(mx);
        _log("thread " << std::this_thread::get_id() << ": " << "started" << std::endl);
        while(!q) {
            _log("thread " << std::this_thread::get_id() << ": wait" << std::endl);
            sleep = true;
            cv.wait(lk, [&] { return !sleep; } );
            _log("thread " << std::this_thread::get_id() << ": " << "work" << std::endl);
            write();
        }
    }

    void quite() {
        q = true;
    }
};

class report;

class log_out : public out_base {
public:
    log_out(report * rp);
    void write() override {
        std::cout << "bulk:";
        for(auto el = ls.begin(); el != ls.end(); el++) {
            if(el != ls.begin()) std::cout << ",";
            std::cout << " " << *el;
        }
        std::cout << std::endl;
    }
};

class write_out : public out_base {
    int ind;
    static int id_cnt; //число объектов

public:
    write_out(report * rp);
    void write() override
    {
        //механизм поочередной обработки событий
        ind = (ind + 1) % id_cnt;
        if(ind % id_cnt != 1) {
            return;
        }

        //get id
        std::stringstream id_ss;
        id_ss << std::this_thread::get_id();

        //create file name
        auto cnt = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
        auto name = std::to_string(cnt.count());
        auto file_name = "bulk" + name + id_ss.str() + ".log";

        std::ofstream myfile;
        myfile.open(file_name);
        _log("file: " << file_name << " content: ");
        for(const auto & el : ls) {
            myfile << el << std::endl;
            _log(el);
        }
        _log(std::endl);
        myfile.close();
    }
};

int write_out::id_cnt = 0;

class report {
    std::list<out_base *> subs;
public:
    void subscribe(out_base * ptr) {
        subs.push_back(ptr);
    }
    void notify_all(cmd_list_t val, time_point_t time) {
        for(auto & el : subs) {
            el->signal(val, time);
        }
    }
};

log_out::log_out(report * rp) {
    rp->subscribe(this);
}

write_out::write_out(report * rp) {
    ind = id_cnt++;
    _log("create write_file with id = " << ind << ", ");
    _log("cnt = " << id_cnt << std::endl);
    rp->subscribe(this);
}
