# ifndef MSC_NANOSLEEP_HANDLER_H
# define MSC_NANOSLEEP_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_nanosleep

class Nanosleep: public Handler {
public:
    Nanosleep() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Nanosleep();
    };
};

REGISTER_HANDLER(Nanosleep, (int) TYPE, Nanosleep::create);
#endif