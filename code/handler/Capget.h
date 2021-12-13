# ifndef MSC_CAPGET_HANDLER_H
# define MSC_CAPGET_HANDLER_H

#include <sys/ptrace.h>
#include "Handler.h"
#include "HandlerRegistry.h"
#include <linux/capability.h> // for cap_user_header t and cap_user_data_t

#define TYPE SYS_capget

class Capget: public Handler {
public:
    Capget() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Capget();
    };
};

REGISTER_HANDLER(Capget, (int) TYPE, Capget::create);

#endif