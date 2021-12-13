# ifndef MSC_EXITGROUP_HANDLER_H
# define MSC_EXITGROUP_HANDLER_H

#include <sys/ptrace.h>
#include "HandlerRegistry.h"

#define TYPE SYS_exit_group

class ExitGroup: public Handler {
public:
    ExitGroup() {
        _isExitCall = true;
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new ExitGroup();
    };
};

REGISTER_HANDLER(ExitGroup, (int) TYPE, ExitGroup::create);

#endif