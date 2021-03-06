# ifndef MSC_KILL_HANDLER_H
# define MSC_KILL_HANDLER_H
// automatically generated by addhandler.py

#include "HandlerRegistry.h"

#define TYPE SYS_kill

class Kill: public Handler {
public:
    Kill() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Kill();
    };
};

REGISTER_HANDLER(Kill, (int) TYPE, Kill::create);

#endif