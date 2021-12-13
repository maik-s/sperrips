# ifndef MSC_GETSOCKNAME_HANDLER_H
# define MSC_GETSOCKNAME_HANDLER_H

#include <sys/socket.h> // struct sockaddr, msghdr

#include "HandlerRegistry.h"

#define TYPE SYS_getsockname

class GetSockName: public Handler {
public:
    GetSockName() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new GetSockName();
    };
};

REGISTER_HANDLER(GetSockName, (int) TYPE, GetSockName::create);

#endif