# ifndef MSC_WRITE_HANDLER_H
# define MSC_WRITE_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_write

class Write: public Handler {
public:
    Write() {
        _type = TYPE;
    }
    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Write();
    };
};

REGISTER_HANDLER(Write, (int) TYPE, Write::create);

#endif