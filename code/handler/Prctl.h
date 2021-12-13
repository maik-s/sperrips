# ifndef MSC_PRCTL_HANDLER_H
# define MSC_PRCTL_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_prctl


class Prctl: public Handler {
public:
    Prctl() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Prctl();
    };
};

REGISTER_HANDLER(Prctl, (int) TYPE, Prctl::create);
#endif