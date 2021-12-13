# ifndef MSC_FADVISE64_HANDLER_H
# define MSC_FADVISE64_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_fadvise64

class Fadvise64: public Handler {
public:
    Fadvise64() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Fadvise64();
    };
};

REGISTER_HANDLER(Fadvise64, (int) TYPE, Fadvise64::create);

#endif