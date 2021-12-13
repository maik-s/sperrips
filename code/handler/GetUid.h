# ifndef MSC_GETUID_HANDLER_H
# define MSC_GETUID_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_getuid

class GetUid: public Handler {
public:
    GetUid() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetUid();
    };
};

REGISTER_HANDLER(GetUid, (int) TYPE, GetUid::create);

#endif