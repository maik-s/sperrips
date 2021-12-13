# ifndef MSC_UNLINK_HANDLER_H
# define MSC_UNLINK_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_unlink

class Unlink: public Handler {
public:
    Unlink() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Unlink();
    };
};

REGISTER_HANDLER(Unlink, (int) TYPE, Unlink::create);


#endif