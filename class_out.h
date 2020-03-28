#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <sstream>
#include <condition_variable>

#include "user_types.h"

const bool test_log = false;
static std::mutex log_mx;
#define _log(x) if(test_log) { \
    std::lock_guard<std::mutex> g(log_mx); \
    std::cout << x; \
    }

//fibbonachi
auto fib(int n) {
    if(n<=2) return 1;
    return fib(n-1) + fib(n-2);
}

//factorial
auto fact(int n) {
    if(n == 0) return 1;
    return n * fact(n-1);
}

class out_base {
protected:
    //метрики
    int blocks_cnt = 0;
    int cmd_cnt = 0;

    cmd_list_t ls; //список полученных блоков
    std::mutex ls_mx;
    time_point_t tp; //время получения блока
    std::mutex tp_mx;
    //для потока
    std::mutex thread_mx;
    std::condition_variable cv;
    bool sleep = true;
    bool q = false;

    virtual void write() = 0;

public:
    virtual ~out_base() = default;
    virtual void signal(cmd_list_t s, time_point_t t) {
        //запоминаем полученные данные
        tp_mx.lock();
        ls_mx.lock();
        ls = s;
        tp = t;
        tp_mx.unlock();
        ls_mx.unlock();
        //метрики
        blocks_cnt++;
        cmd_cnt += s.size();
        //пробуждаем поток
        sleep = false;
        cv.notify_one();
    }
    auto thread_exec() {
        std::unique_lock<std::mutex> lk(thread_mx);
        _log("thread " << std::this_thread::get_id() << ": " << "started" << std::endl)
        while(!q) {
            _log("thread " << std::this_thread::get_id() << ": wait" << std::endl)
            sleep = true;
            cv.wait(lk, [&] { return !sleep || q; } );
            _log("thread " << std::this_thread::get_id() << ": " << "wake up" << std::endl)
            if(!sleep) write();
        }
        _log("thread " << std::this_thread::get_id() << ": " << "quit" << std::endl)
        return std::tuple(cmd_cnt, blocks_cnt);
    }
    void drop() {
        q = true;
        cv.notify_one();
    }
};

class report;

class log_out : public out_base {
public:
    log_out(report * rp);
    void write() override {
        std::cout << "bulk:";
        ls_mx.lock();
        for(auto el = ls.begin(); el != ls.end(); el++) {
            if(el != ls.begin()) std::cout << ",";
            std::cout << " " << fact(std::stoi(*el));
        }
        ls_mx.unlock();
        std::cout << std::endl;
    }
};

class write_out : public out_base {
    int id;
    int ind;
    static int id_cnt; //число объектов

public:

    void signal(cmd_list_t s, time_point_t t) override {
        //механизм поочередной обработки событий
        ind = (ind + 1) % id_cnt;
        if(ind % id_cnt != 1) {
            _log("file_obj. id = " << id << " rest" << std::endl)
            return;
        }
        _log("file_obj. id = " << id << " write" << std::endl)

        out_base::signal(s, t);
    }

    write_out(report * rp);
    void write() override
    {
        //get id
        std::stringstream id_ss;
        id_ss << std::this_thread::get_id();

        //create file name
        tp_mx.lock();
        auto cnt = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
        tp_mx.unlock();
        auto name = std::to_string(cnt.count());
        auto file_name = "bulk" + name + "_" + id_ss.str() + ".log";

        std::ofstream myfile;
        myfile.open(file_name);
        _log("file: " << file_name << " content: ")
        ls_mx.lock();
        for(const auto & el : ls) {
            auto val = fib(std::stoi(el));
            myfile << val << std::endl;
            _log(val)
        }
        ls_mx.unlock();
        _log(std::endl)
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
    void drop_all() {
        for(auto & el : subs) {
            el->drop();
        }
    }
};

log_out::log_out(report * rp) {
    rp->subscribe(this);
}

write_out::write_out(report * rp) {
    id = id_cnt++;
    ind = id;
    _log("create file_obj. id = " << id << ", " <<
         "cnt = " << id_cnt << std::endl)
    rp->subscribe(this);
}
