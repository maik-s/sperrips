# ifndef MSC_READ_HANDLER_H
# define MSC_READ_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_read

class Read: public Handler {
public:
    Read() {
        _type = TYPE;
    }
    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Read();
    };
};

REGISTER_HANDLER(Read, (int) TYPE, Read::create);

#endif