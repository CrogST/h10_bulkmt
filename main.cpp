
#include <optional>
#include <vector>
#include <chrono>
#include <future>

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

    auto t1 = std::async(std::launch::async, &out_base::thread_exec, &fl1);
    auto t2 = std::async(std::launch::async, &out_base::thread_exec, &fl2);
    auto t3 = std::async(std::launch::async, &out_base::thread_exec, &log);

    auto notify_handler = [&](res_t res) {
        auto val = res.value();
        auto ls = std::get<0>(val);
        auto tp = std::get<1>(val);
        log.signal(ls, tp);
        fl1.signal(ls, tp);
        fl2.signal(ls, tp);
    };

    int line_cnt = 0;
    for(std::string line; std::getline(std::cin, line); )
    {
        if(line.length() == 0) break;
        line_cnt++;
        auto res = col.handle(line);
        if(res) notify_handler(res);
    }
    auto res = col.get_now();
    if(res) notify_handler(res);

    fl1.quite();
    fl2.quite();
    log.quite();

    int cmd;
    int blocks;
    std::tie(cmd, blocks) = t1.get();
    std::cout << "file1 thread. " << "blocks: " << blocks << ", " << "commands: " << cmd << std::endl;
    std::tie(cmd, blocks) = t2.get();
    std::cout << "file2 thread. " << "blocks: " << blocks << ", " << "commands: " << cmd << std::endl;
    std::tie(cmd, blocks) = t3.get();
    std::cout << "log thread. " << "blocks: " << blocks << ", " << "commands: " << cmd << std::endl;

    std::cout << "main thread. "
              << "blocks: " << blocks << ", "
              << "commands: " << cmd << ", "
              << "lines: " << line_cnt
              << std::endl;

    return 0;
}
