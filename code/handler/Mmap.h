# ifndef MSC_MMAP_HANDLER_H
# define MSC_MMAP_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_mmap

class Mmap: public Handler {
public:
    Mmap() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    argument_type getReturnType();

    static Handler* create() {
        return new Mmap();
    };
};

REGISTER_HANDLER(Mmap, (int) TYPE, Mmap::create);

#endif