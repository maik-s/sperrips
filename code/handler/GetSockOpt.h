# ifndef MSC_GETSOCKOPT_HANDLER_H
# define MSC_GETSOCKOPT_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_getsockopt

class GetSockOpt: public Handler {
public:
    GetSockOpt() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetSockOpt();
    };
};

REGISTER_HANDLER(GetSockOpt, (int) TYPE, GetSockOpt::create);

#endif