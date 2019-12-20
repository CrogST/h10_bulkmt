
#include <optional>
#include <vector>
#include <chrono>

#include "collect.h"
#include "class_out.h"


int main(int argc, char *argv[])
{
    if(argc <= 1) return -1;
    unsigned int n = static_cast<unsigned int>(std::stoi(argv[1]));

    report rp;
    log_out log(&rp);
    write_out fl1(&rp);
    write_out fl2(&rp);

    collect col{n};

    std::thread t1(&out_base::thread_exec, &fl1);
    std::thread t2(&out_base::thread_exec, &fl2);
    std::thread t3(&out_base::thread_exec, &log);

    auto notify_handler = [&](res_t res) {
        auto val = res.value();
        auto ls = std::get<0>(val);
        auto tp = std::get<1>(val);
        log.signal(ls, tp);
        fl1.signal(ls, tp);
        fl2.signal(ls, tp);
    };

    for(std::string line; std::getline(std::cin, line); )
    {
        auto res = col.handle(line);
        if(res) notify_handler(res);
    }
    auto res = col.get_now();
    if(res) notify_handler(res);

    fl1.quite();
    fl2.quite();
    log.quite();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
