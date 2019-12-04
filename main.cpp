
#include <optional>

#include "collect.h"
#include "class_out.h"

#include <vector>

int main(int argc, char *argv[])
{
    if(argc <= 1) return -1;
    unsigned int n = static_cast<unsigned int>(std::stoi(argv[1]));

    report rp;
    log_out log(&rp);
    write_out fl(&rp);

    collect col{n};

    {

        std::vector<std::string> vct = {
            "{",
            "00",
            "01",
            "02",
            "03",
            "04",
            "}",
            "000",
            "001",
            "002",
            "003",
            "{",
            "0000",
            "0001",
            "0002",
            "0003",
            "0004",
            "}"
        };

        for(const auto & str : vct) {
            auto res = col.handle(str);
            if(res) {
                auto val = res.value();
                rp.notify_all(std::get<0>(val), std::get<1>(val));
            }
        }
    }

    for(std::string line; std::getline(std::cin, line); )
    {
        auto res = col.handle(line);
        if(res) {
            auto val = res.value();
            rp.notify_all(std::get<0>(val), std::get<1>(val));
        }
    }

    auto res = col.get_now();
    if(res) {
        auto val = res.value();
        rp.notify_all(std::get<0>(val), std::get<1>(val));
    }

    return 0;
}
