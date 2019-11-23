#pragma once

#include <list>
#include <optional>

using cmd_list_t = std::list<std::string>;
using res_t = std::optional<std::string>;

class collect {
    bool first;
    unsigned int N;
    cmd_list_t ls;
    enum type {
        collect_n,
        collect_wait
    } handle_type; //want to make it func_ptr, but can't do it.

    std::string convert(cmd_list_t ls);
    res_t collect_clever(std::string val);
    res_t collect_N(std::string val);

public:
    collect();
    res_t handle(std::string str);
};
