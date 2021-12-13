# ifndef MSC_FCHMOD_HANDLER_H
# define MSC_FCHMOD_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_fchmod

class FChmod: public Handler {
public:
    FChmod() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new FChmod();
    };
};

REGISTER_HANDLER(FChmod, (int) TYPE, FChmod::create);

#endif