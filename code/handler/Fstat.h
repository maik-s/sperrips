# ifndef MSC_FSTAT_HANDLER_H
# define MSC_FSTAT_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_fstat
#include <sys/stat.h> //struct stat

class Fstat: public Handler {
public:
    Fstat() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Fstat();
    };
};

REGISTER_HANDLER(Fstat, (int) TYPE, Fstat::create);

#endif