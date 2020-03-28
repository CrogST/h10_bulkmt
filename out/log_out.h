#ifndef LOG_OUT_H
#define LOG_OUT_H

#include "report.h"
#include "out_base.h"

class log_out : public out_base {
    //factorial
    int fact(int n);
public:
    log_out(report * rp);
    void write() override;
};

#endif // LOG_OUT_H
