# ifndef MSC_UMASK_HANDLER_H
# define MSC_UMASK_HANDLER_H
// automatically generated by addhandler.py

#include "HandlerRegistry.h"

#define TYPE SYS_umask

class Umask: public Handler {
public:
    Umask() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Umask();
    };
};

REGISTER_HANDLER(Umask, (int) TYPE, Umask::create);

#endif