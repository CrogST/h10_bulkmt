
#include "collect.h"

res_t collect::done() {
    cmd_list_t res = std::move(ls);
    ls.clear();
    return std::move(res);
}

res_t collect::collect_clever(std::string val) {
    static int deep_cnt = 1;
    if(val.size() == 1) {
        if(val[0] == '{') {
            deep_cnt++;
        } else if(val[0] == '}') {
            deep_cnt--;
            if(deep_cnt == 0) {
                handle_type = type::collect_n;
                return done();
            }
        } else {
            ls.push_back(val);
        }
    } else {
        ls.push_back(val);
    }
    return std::nullopt;
}

res_t collect::collect_N(std::string val) {

    if(first) {
        first = false;
        N = static_cast<unsigned int>(std::stoi(val));
    } else {
        if(val.size() == 1 && val[0] == '{') {
            handle_type = type::collect_wait;
            return done();
        } else {
            ls.push_back(val);
            if(ls.size() >= N) {
                return done();
            }
        }
    }
    return std::nullopt;
}

collect::collect() {
    first = true;
    handle_type = type::collect_n;
}

res_t collect::handle(std::string str) {
    switch (handle_type) {
    case type::collect_n: return collect_N(str);
    case type::collect_wait: return collect_clever(str);
    }
}

res_t collect::get_now() {
    if(handle_type == type::collect_n)
        return done();
    else return std::nullopt;
}
