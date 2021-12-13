# ifndef MSC_LSEEK_HANDLER_H
# define MSC_LSEEK_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_lseek

class Lseek: public Handler {
public:
    Lseek() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Lseek();
    };
};

REGISTER_HANDLER(Lseek, (int) TYPE, Lseek::create);

#endif