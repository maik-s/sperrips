# ifndef MSC_UNAME_HANDLER_H
# define MSC_UNAME_HANDLER_H

#include <sys/utsname.h> // struct utsname
#include "HandlerRegistry.h"

#define TYPE SYS_uname

class Uname: public Handler {
public:
    Uname() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Uname();
    };
};

REGISTER_HANDLER(Uname, (int) TYPE, Uname::create);

#endif