# ifndef MSC_RTSIGACTION_HANDLER_H
# define MSC_RTSIGACTION_HANDLER_H

#include <signal.h> // struct sigaction
#include "HandlerRegistry.h"

#define TYPE SYS_rt_sigaction

class RtSigaction: public Handler {
public:
    RtSigaction() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new RtSigaction();
    };
};

REGISTER_HANDLER(RtSigaction, (int) TYPE, RtSigaction::create);
#endif