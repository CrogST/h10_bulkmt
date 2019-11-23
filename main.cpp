
#include <optional>

#include "collect.h"
#include "class_out.h"

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
