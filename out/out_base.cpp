#include "out_base.h"

#include <thread>

#include "dbg_log.h"

void out_base::signal(cmd_list_t s, time_point_t t) {
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
std::tuple<int, int> out_base::thread_exec() {
    std::unique_lock<std::mutex> lk(thread_mx);
    dbg_log("thread " << std::this_thread::get_id() << ": " << "started" << std::endl)
    while(!q) {
        dbg_log("thread " << std::this_thread::get_id() << ": wait" << std::endl);
        sleep = true;
        cv.wait(lk, [&] { return !sleep || q; });
        dbg_log("thread " << std::this_thread::get_id() << ": " << "wake up" << std::endl)

        if(!sleep) { write(); }
    }
    dbg_log("thread " << std::this_thread::get_id() << ": " << "quit" << std::endl)
    return std::tuple(cmd_cnt, blocks_cnt);
}
void out_base::drop() {
    q = true;
    cv.notify_one();
}

