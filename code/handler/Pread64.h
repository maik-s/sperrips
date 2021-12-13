# ifndef MSC_PREAD64_HANDLER_H
# define MSC_PREAD64_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_pread64

class Pread64: public Handler {
public:
    Pread64() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Pread64();
    };
};

REGISTER_HANDLER(Pread64, (int) TYPE, Pread64::create);
#endif