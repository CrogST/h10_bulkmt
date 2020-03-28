#pragma once

#include <tuple>
#include <mutex>
#include <condition_variable>

#include "user_types.h"

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
    std::mutex sleep_mx;
    bool q = false;
    std::mutex q_mx;

    virtual void write() = 0;

public:
    virtual ~out_base() = default;
    virtual void signal(cmd_list_t s, time_point_t t);
    std::tuple<int, int> thread_exec();
    void drop();
};
