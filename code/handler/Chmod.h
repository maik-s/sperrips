# ifndef MSC_CHMOD_HANDLER_H
# define MSC_CHMOD_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_chmod

class Chmod: public Handler {
public:
    Chmod() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Chmod();
    };
};
#endif