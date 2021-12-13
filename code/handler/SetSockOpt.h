# ifndef MSC_SETSOCKOPT_HANDLER_H
# define MSC_SETSOCKOPT_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_setsockopt

class SetSockOpt: public Handler {
public:
    SetSockOpt() {
        _type = TYPE;
    }
    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SetSockOpt();
    };
};

REGISTER_HANDLER(SetSockOpt, (int) TYPE, SetSockOpt::create);

#endif