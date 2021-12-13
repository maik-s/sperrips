# ifndef MSC_SIGPROCMASK_HANDLER_H
# define MSC_SIGPROCMASK_HANDLER_H


#include "HandlerRegistry.h"

#define TYPE SYS_rt_sigprocmask

class SigProcMask: public Handler {
public:
    SigProcMask() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SigProcMask();
    };

};

REGISTER_HANDLER(SigProcMask, (int) TYPE, SigProcMask::create);

#endif