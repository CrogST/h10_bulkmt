#pragma once

#include <list>
#include <optional>

#include "user_types.h"

using res_t = std::optional<cmd_list_t>;

class collect {
    unsigned int N; /// \brief задаваемый размер блока
    cmd_list_t ls;
    enum type {
        collect_n,
        collect_wait
    } handle_type; //want to make it func_ptr, but it wasn't work

    res_t done();
    res_t collect_clever(std::string val);
    res_t collect_N(std::string val);

public:
    collect(unsigned int n);
    res_t handle(std::string str);
    res_t get_now();
};
