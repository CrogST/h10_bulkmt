
#include "collect.h"

res_opt_t collect::done() {
    if(ls.size() == 0) return std::nullopt;
    auto res = std::make_pair(std::move(ls), save_time);
    ls.clear();
    return res;
}

res_opt_t collect::collect_clever(std::string val) {
    if(val.compare("{") == 0) {
        deep_cnt++;
    } else if(val.compare("}") == 0) {
        deep_cnt--;
        if(deep_cnt == 0) {
            handle_type = type::collect_n;
            return done();
        }
    } else {
        if(ls.size() == 0)
            save_time = std::chrono::system_clock::now();
        ls.push_back(val);
    }
    return std::nullopt;
}

res_opt_t collect::collect_N(std::string val) {
    if(val.compare("{") == 0) {
        handle_type = type::collect_wait;
        collect_clever(val);
        return done();
    } else {
        if(ls.size() == 0)
            save_time = std::chrono::system_clock::now();
        ls.push_back(val);
        if(ls.size() >= N) {
            return done();
        }
    }
    return std::nullopt;
}

collect::collect(unsigned int n) : N(n) {
    handle_type = type::collect_n;
}

res_opt_t collect::handle(std::string str) {
    switch (handle_type) {
    case type::collect_n: return collect_N(str);
    case type::collect_wait: return collect_clever(str);
    }
}

res_opt_t collect::get_rest() {
    if(handle_type == type::collect_n)
        return done();
    else return std::nullopt;
}
