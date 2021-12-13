# ifndef MSC_CHDIR_HANDLER_H
# define MSC_CHDIR_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_chdir

class Chdir: public Handler {
public:
    Chdir() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Chdir();
    };
};

REGISTER_HANDLER(Chdir, (int) TYPE, Chdir::create);

#endif