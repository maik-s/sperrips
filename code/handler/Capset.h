# ifndef MSC_CAPSET_HANDLER_H
# define MSC_CAPSET_HANDLER_H

#include <linux/capability.h> // for cap_user_header t and cap_user_data_t
#include "HandlerRegistry.h"

#define TYPE SYS_capset

class Capset: public Handler {
public:
    Capset() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Capset();
    };
};

REGISTER_HANDLER(Capset, (int) TYPE, Capset::create);
#endif