#pragma once

#include <optional>

#include "user_types.h"

using res_t = std::pair<cmd_list_t, time_point_t>;
using res_opt_t = std::optional<res_t>;

class collect {
    unsigned int N; /// \brief задаваемый размер блока
    cmd_list_t ls;
    int deep_cnt = 0;
    time_point_t save_time;

    enum mode {
        n,
        block
    } handle_mode; //want to make it func_ptr, but it wasn't work

    res_opt_t done();
    res_opt_t collect_clever(std::string val);
    res_opt_t collect_N(std::string val);

public:
    collect(unsigned int n);
    res_opt_t handle(std::string str);
    res_opt_t get_rest();
};
