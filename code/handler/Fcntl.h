# ifndef MSC_FCNTL_HANDLER_H
# define MSC_FCNTL_HANDLER_H

#include <fcntl.h> // struct flock
#include "HandlerRegistry.h"

#define TYPE SYS_fcntl

class Fcntl: public Handler {
public:
    Fcntl() {
        _type = TYPE;
        _argumentsDepend = true;
    }

    std::vector<argument_type> getArgumentTypes();

    void rewriteArgumentType(int argumentIndex, argument_value argumentValue);

    static Handler* create() {
        return new Fcntl();
    };
};

REGISTER_HANDLER(Fcntl, (int) TYPE, Fcntl::create);

#endif