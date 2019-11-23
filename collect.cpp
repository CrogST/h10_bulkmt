
#include "collect.h"

std::string collect::convert(cmd_list_t ls) {
    std::string str;
    for(const auto & el : ls) {
        str += el + " ";
    }
    return str;
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
                auto res = convert(ls);
                ls.clear();
                return std::move(res);
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
            //и начать накапливать
            auto res = convert(ls);
            ls.clear();
            return std::move(res);
        } else {
            ls.push_back(val);
            if(ls.size() >= N) {
                auto res = convert(ls);
                ls.clear();
                return std::move(res);
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
