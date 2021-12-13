# ifndef MSC_GETPID_HANDLER_H
# define MSC_GETPID_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_getpid

class GetPid: public Handler {
public:
    GetPid() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetPid();
    };
};

REGISTER_HANDLER(GetPid, (int) TYPE, GetPid::create);

#endif