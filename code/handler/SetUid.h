# ifndef MSC_SETUID_HANDLER_H
# define MSC_SETUID_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_setuid

class SetUid: public Handler {
public:
    SetUid() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SetUid();
    };
};

REGISTER_HANDLER(SetUid, (int) TYPE, SetUid::create);

#endif