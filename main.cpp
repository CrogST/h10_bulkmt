
#include <optional>

#include "collect.h"
#include "class_out.h"

int main(int argc, char *argv[])
{
    if(argc <= 1) return -1;
    unsigned int n = static_cast<unsigned int>(std::stoi(argv[1]));

    report rp;
    log_out log(&rp);
    write_out fl(&rp);

    collect col{n};
    for(std::string line; std::getline(std::cin, line); )
    {
        auto res = col.handle(line);
        if(res) rp.notify_all(res.value());
    }

    auto res = col.get_now();
    if(res) rp.notify_all(res.value());

    return 0;
}
