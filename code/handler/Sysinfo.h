# ifndef MSC_SYSINFO_HANDLER_H
# define MSC_SYSINFO_HANDLER_H

#include <sys/sysinfo.h> // struct sysinfo
#include "HandlerRegistry.h"

#define TYPE SYS_sysinfo

class Sysinfo: public Handler {
public:
    Sysinfo() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Sysinfo();
    };
};

REGISTER_HANDLER(Sysinfo, (int) TYPE, Sysinfo::create);

#endif