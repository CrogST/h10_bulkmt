
/* Ввод строк.
 * Каждая nая строка вывод
 * Если начались скобки - др. состояния.
 * Два подписчика на вывод: запись в файл и вывод в лог.
 * */

#include "class_out.h"
#include <optional>

static bool first = true;
static unsigned int N = 0;
static cmd_list_t ls;
std::optional<std::string> collect_N(std::string val);
static auto ptr_func = collect_N;

std::string convert(cmd_list_t ls) {
    std::string str;
    for(const auto & el : ls)
        str += el + " ";
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
                ptr_func = collect_N;
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
            std::optional<std::string> collect_clever(std::string val);
            ptr_func = collect_clever;
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

int main()
{
    report rp;
    log_out log(&rp);
    write_out fl(&rp);

    for(std::string line; std::getline(std::cin, line); )
    {
        auto res = ptr_func(line);
        if(res) rp.notify_all(res.value());
    }

    return 0;
}
