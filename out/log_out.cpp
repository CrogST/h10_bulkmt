#include "log_out.h"

#include <iostream>

log_out::log_out(report * rp) {
    rp->subscribe(this);
}

int log_out::fact(int n) {
    if(n == 0) return 1;
    return n * fact(n-1);
}

void log_out::write() {
    std::cout << "bulk:";
    ls_mx.lock();
    for(auto el = ls.begin(); el != ls.end(); el++) {
        if(el != ls.begin()) std::cout << ",";
        std::cout << " " << fact(std::stoi(*el));
    }
    ls_mx.unlock();
    std::cout << std::endl;
}
