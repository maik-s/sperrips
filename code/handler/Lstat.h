# ifndef MSC_LSTAT_HANDLER_H
# define MSC_LSTAT_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_lstat

#include <sys/stat.h> //struct stat

class Lstat: public Handler {
public:
    Lstat() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Lstat();
    };

};

REGISTER_HANDLER(Lstat, (int) TYPE, Lstat::create);

#endif