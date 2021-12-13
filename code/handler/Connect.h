# ifndef MSC_CONNECT_HANDLER_H
# define MSC_CONNECT_HANDLER_H

#include <sys/socket.h> // struct sockaddr, msghdr
#include "HandlerRegistry.h"

#define TYPE SYS_connect

class Connect: public Handler {
public:
    Connect() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Connect();
    };
};

REGISTER_HANDLER(Connect, (int) TYPE, Connect::create);

#endif