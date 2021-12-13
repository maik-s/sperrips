# ifndef MSC_IOCTL_HANDLER_H
# define MSC_IOCTL_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_ioctl

class Ioctl: public Handler {
public:
    Ioctl() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Ioctl();
    };
};

REGISTER_HANDLER(Ioctl, (int) TYPE, Ioctl::create);

#endif