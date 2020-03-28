#ifndef WRITE_OUT_H
#define WRITE_OUT_H

#include "out_base.h"
#include "report.h"

class write_out : public out_base {
    int id;
    int ind;
    static int id_cnt; //число объектов
    static std::mutex id_cnt_mx;

    //fibbonachi
    int fib(int n);
public:
    void signal(cmd_list_t s, time_point_t t);
    write_out(report * rp);
    void write() override;
};

#endif // WRITE_OUT_H
