
#include <optional>
#include <vector>
#include <chrono>
#include <future>

#include "collect.h"
#include "class_out.h"


int main(int argc, char *argv[])
{
    if(argc <= 1) {
        std::cout << "no arguments were provided" << std::endl;
        return -1;
    }
    unsigned int n = static_cast<unsigned int>(std::stoi(argv[1]));

    report rp;

    using async_t = decltype(
    std::async(std::launch::async, &out_base::thread_exec, write_out(&rp)));

    std::list<async_t> threads;

    write_out fl1(&rp);
    threads.emplace_back(
                std::async(
                    std::launch::async,
                    &out_base::thread_exec, std::ref(fl1)));
    write_out fl2(&rp);
    threads.emplace_back(
                std::async(
                    std::launch::async,
                    &out_base::thread_exec, std::ref(fl2)));
    log_out log(&rp);
    threads.emplace_back(
                std::async(
                    std::launch::async,
                    &out_base::thread_exec, std::ref(log)));
    auto & log_thread = threads.back();

    auto notify_handler = [&](auto val) {
        auto ls = std::get<0>(val);
        auto tp = std::get<1>(val);
        rp.notify_all(ls, tp);
    };

    int line_cnt = 0;
    collect col{n};
    for(std::string line; std::getline(std::cin, line); )
    {
        if(line.length() == 0) break;
        line_cnt++;
        auto res = col.handle(line);
        if(res) notify_handler(res.value());
    }
    auto res = col.get_rest();
    if(res) notify_handler(res.value());

    rp.drop_all();

    int cmd;
    int blocks;
    unsigned int cnt = 0;
    for(auto & el : threads) {
        cnt++;
        std::tie(cmd, blocks) = el.get();
        std::cout << "thread " << cnt << ". " << "blocks: " << blocks << ", " << "commands: " << cmd << std::endl;
    }

    std::tie(cmd, blocks) = log_thread.get();
    std::cout << "main thread. "
              << "blocks: " << blocks << ", "
              << "commands: " << cmd << ", "
              << "lines: " << line_cnt
              << std::endl;

    return 0;
}
