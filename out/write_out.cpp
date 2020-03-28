#include "write_out.h"

#include "report.h"
#include "dbg_log.h"

#include <fstream>
#include <thread>
#include <sstream>
#include <iostream>


int write_out::id_cnt = 0;
std::mutex write_out::id_cnt_mx;

write_out::write_out(report * rp) {
    id_cnt_mx.lock();
    id = id_cnt++;
    id_cnt_mx.unlock();
    ind = id;
    dbg_log("create file_obj. id = " << id << ", " <<
         "cnt = " << id_cnt << std::endl)
    rp->subscribe(this);
}

void write_out::signal(cmd_list_t s, time_point_t t) {
    id_cnt_mx.lock();
    auto cnt = id_cnt;
    id_cnt_mx.unlock();
    //механизм поочередной обработки событий
    ind = (ind + 1) % cnt;
    if(ind % cnt != 1) {
        dbg_log("file_obj. id = " << id << " rest" << std::endl)
        return;
    }
    dbg_log("file_obj. id = " << id << " write" << std::endl)

    out_base::signal(s, t);
}

int write_out::fib(int n) {
    if(n<=2) return 1;
    return fib(n-1) + fib(n-2);
}

void write_out::write()
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
    dbg_log("file: " << file_name << " content: ")
    ls_mx.lock();
    for(const auto & el : ls) {
        auto val = fib(std::stoi(el));
        myfile << val << std::endl;
        dbg_log(val)
    }
    ls_mx.unlock();
    dbg_log(std::endl)
    myfile.close();
}
