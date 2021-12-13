# ifndef MSC_ACCESS_HANDLER_H
# define MSC_ACCESS_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_access

class Access: public Handler {
public:
    Access() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Access();
    };
};

REGISTER_HANDLER(Access, (int) TYPE, Access::create);

#endif