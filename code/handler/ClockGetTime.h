# ifndef MSC_CLOCKGETTIME_HANDLER_H
# define MSC_CLOCKGETTIME_HANDLER_H
// automatically generated by addhandler.py

#include "HandlerRegistry.h"

#define TYPE SYS_clock_gettime

class ClockGetTime: public Handler {
public:
    ClockGetTime() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new ClockGetTime();
    };
};

REGISTER_HANDLER(ClockGetTime, (int) TYPE, ClockGetTime::create);

#endif