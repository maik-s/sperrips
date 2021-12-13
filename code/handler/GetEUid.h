# ifndef MSC_GETEUID_HANDLER_H
# define MSC_GETEUID_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_geteuid

class GetEUid: public Handler {
public:
    GetEUid() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetEUid();
    };
};

REGISTER_HANDLER(GetEUid, (int) TYPE, GetEUid::create);

#endif