# ifndef MSC_FCHDIR_HANDLER_H
# define MSC_FCHDIR_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_fchdir

class FChdir: public Handler {
public:
    FChdir() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new FChdir();
    };
};

REGISTER_HANDLER(FChdir, (int) TYPE, FChdir::create);

#endif