# ifndef MSC_SETXATTR_HANDLER_H
# define MSC_SETXATTR_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_setxattr

class SetXAttr: public Handler {
public:
    SetXAttr() {
        _type = TYPE;
    }
    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SetXAttr();
    };

};

REGISTER_HANDLER(SetXAttr, (int) TYPE, SetXAttr::create);

#endif