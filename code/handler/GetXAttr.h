# ifndef MSC_GETXATTR_HANDLER_H
# define MSC_GETXATTR_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_getxattr

class GetXAttr: public Handler {
public:
    GetXAttr() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetXAttr();
    };
};

REGISTER_HANDLER(GetXAttr, (int) TYPE, GetXAttr::create);

#endif