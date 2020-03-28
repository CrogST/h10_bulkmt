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
    sleep_mx.lock();
    sleep = false;
    sleep_mx.unlock();
    cv.notify_one();
}
std::tuple<int, int> out_base::thread_exec() {
    std::unique_lock<std::mutex> lk(thread_mx);
    _log("thread " << std::this_thread::get_id() << ": " << "started" << std::endl)
    q_mx.lock();
    while(!q) {
        q_mx.unlock();
        _log("thread " << std::this_thread::get_id() << ": wait" << std::endl);

        sleep_mx.lock();
        sleep = true;
        sleep_mx.unlock();

        cv.wait(lk, [&] {
            std::lock_guard<std::mutex> lck_mx(sleep_mx);
            std::lock_guard<std::mutex> lck_q(q_mx);
            return !sleep || q;
        });
        _log("thread " << std::this_thread::get_id() << ": " << "wake up" << std::endl)

        sleep_mx.lock();
        auto sl = sleep;
        sleep_mx.unlock();
        if(!sl) { write(); }

        q_mx.lock();
    }
    _log("thread " << std::this_thread::get_id() << ": " << "quit" << std::endl)
    return std::tuple(cmd_cnt, blocks_cnt);
}
void out_base::drop() {
    q_mx.lock();
    q = true;
    q_mx.unlock();
    cv.notify_one();
}

