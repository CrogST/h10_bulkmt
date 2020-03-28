#pragma once

#include <iostream>
#include <mutex>

const bool test_log = false;
extern std::mutex dbg_log_mx;
#define dbg_log(x) if (test_log) { \
    std::lock_guard<std::mutex> g(dbg_log_mx); \
    std::cout << x; \
    }
