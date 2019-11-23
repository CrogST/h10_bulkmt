
/* Ввод строк.
 * Каждая nая строка вывод
 * Если начались скобки - др. состояния.
 * Два подписчика на вывод: запись в файл и вывод в лог.
 * */

#include "class_out.h"
#include <optional>

class collect {
    bool first;
    unsigned int N;
    cmd_list_t ls;
    enum type {
        collect_n,
        collect_wait
    };
    type handle_type;

    std::string convert(cmd_list_t ls) {
        std::string str;
        for(const auto & el : ls) {
            str += el + " ";
        }
        return str;
    }

    std::optional<std::string> collect_clever(std::string val) {
        static int deep_cnt = 1;
        if(val.size() == 1) {
            if(val[0] == '{') {
                deep_cnt++;
            } else if(val[0] == '}') {
                deep_cnt--;
                if(deep_cnt == 0) {
                    std::optional<std::string> collect_N(std::string val);
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

    std::optional<std::string> collect_N(std::string val) {

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

public:
    collect() {
        first = true;
        handle_type = type::collect_n;
    }

    auto handle(std::string str) {
        switch (handle_type) {
        case type::collect_n: return collect_N(str);
        case type::collect_wait: return collect_clever(str);
        }
    }
};

int main()
{
    report rp;
    log_out log(&rp);
    write_out fl(&rp);

    collect col;
    for(std::string line; std::getline(std::cin, line); )
    {
        auto res = col.handle(line);
        if(res) rp.notify_all(res.value());
    }

    return 0;
}
