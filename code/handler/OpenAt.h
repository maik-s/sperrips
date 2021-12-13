# ifndef MSC_OPENAT_HANDLER_H
# define MSC_OPENAT_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_openat

class OpenAt: public Handler {
public:
    OpenAt() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new OpenAt();
    };
};

REGISTER_HANDLER(OpenAt, (int) TYPE, OpenAt::create);
#endif