# ifndef MSC_MUNMAP_HANDLER_H
# define MSC_MUNMAP_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_munmap

class Munmap: public Handler {
public:
    Munmap() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Munmap();
    };

};

REGISTER_HANDLER(Munmap, (int) TYPE, Munmap::create);

#endif