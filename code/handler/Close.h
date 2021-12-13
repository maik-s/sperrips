# ifndef MSC_CLOSE_HANDLER_H
# define MSC_CLOSE_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_close

class Close: public Handler {
public:
    Close() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Close();
    };

};

REGISTER_HANDLER(Close, (int) TYPE, Close::create);

#endif