# ifndef MSC_SETITIMER_HANDLER_H
# define MSC_SETITIMER_HANDLER_H

#include <sys/time.h> // struct timespec, setitimerval
#include "HandlerRegistry.h"

#define TYPE SYS_setitimer

class SetItimer: public Handler {
public:
    SetItimer() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SetItimer();
    };
};

REGISTER_HANDLER(SetItimer, (int) TYPE, SetItimer::create);

#endif