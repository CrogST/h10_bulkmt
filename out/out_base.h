#pragma once

#include <tuple>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "user_types.h"

class out_base {
    //метрики
    int blocks_cnt = 0;
    int cmd_cnt = 0;
protected:    
    cmd_list_t ls; //список полученных блоков
    std::mutex ls_mx;
    time_point_t tp; //время получения блока
    std::mutex tp_mx;
    //для потока
    std::mutex thread_mx;
    std::condition_variable cv;

    std::atomic_bool sleep = true;
    std::atomic_bool q = false;

    virtual void write() = 0;

public:
    virtual ~out_base() = default;
    virtual void signal(cmd_list_t s, time_point_t t);
    std::tuple<int, int> thread_exec();
    void drop();
};
