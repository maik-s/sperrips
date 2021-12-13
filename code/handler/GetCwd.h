# ifndef MSC_GETCWD_HANDLER_H
# define MSC_GETCWD_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_getcwd

class GetCwd: public Handler {
public:
    GetCwd() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetCwd();
    };
};

REGISTER_HANDLER(GetCwd, (int) TYPE, GetCwd::create);

#endif