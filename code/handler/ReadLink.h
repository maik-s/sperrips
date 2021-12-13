# ifndef MSC_READLINK_HANDLER_H
# define MSC_READLINK_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_readlink

class ReadLink: public Handler {
public:
    ReadLink() {
        _type = TYPE;
    }
    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new ReadLink();
    };
};

REGISTER_HANDLER(ReadLink, (int) TYPE, ReadLink::create);
#endif