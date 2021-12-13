# ifndef MSC_STAT_HANDLER_H
# define MSC_STAT_HANDLER_H

#include <sys/stat.h> //struct stat
#include "HandlerRegistry.h"

#define TYPE SYS_stat

class Stat: public Handler {
public:
    Stat() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Stat();
    };
};

REGISTER_HANDLER(Stat, (int) TYPE, Stat::create);
#endif