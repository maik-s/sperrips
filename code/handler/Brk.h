# ifndef MSC_BRK_HANDLER_H
# define MSC_BRK_HANDLER_H

#include "Handler.h"
#include "HandlerRegistry.h"

#define TYPE SYS_brk

class Brk: public Handler {
public:
    Brk() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();
    argument_type getReturnType();
    static Handler* create() {
        return new Brk();
    };
};

REGISTER_HANDLER(Brk, (int) TYPE, Brk::create);

#endif