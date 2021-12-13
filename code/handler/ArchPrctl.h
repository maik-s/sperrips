# ifndef MSC_ARCH_PRCTL_HANDLER_H
# define MSC_ARCH_PRCTL_HANDLER_H

#include <sys/ptrace.h>
#include "HandlerRegistry.h"

#define TYPE SYS_arch_prctl

class ArchPrctl: public Handler {
public:
    ArchPrctl() {
        _type = TYPE;
        _argumentsDepend = true;
    };

    std::vector<argument_type> getArgumentTypes();

    void rewriteArgumentType(int argumentIndex, argument_value argumentValue);

    static Handler* create() {
        return new ArchPrctl();
    };
};

REGISTER_HANDLER(ArchPrctl, (int) TYPE, ArchPrctl::create);

#endif