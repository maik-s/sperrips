# ifndef MSC_MPROTECT_HANDLER_H
# define MSC_MPROTECT_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_mprotect

class Mprotect: public Handler {
public:
    Mprotect() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Mprotect();
    };
};

REGISTER_HANDLER(Mprotect, (int) TYPE, Mprotect::create);

#endif