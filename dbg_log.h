#pragma once

#include <iostream>
#include <mutex>

const bool test_log = false;
extern std::mutex log_mx;
#define _log(x) if (test_log) { \
    std::lock_guard<std::mutex> g(log_mx); \
    std::cout << x; \
    }
