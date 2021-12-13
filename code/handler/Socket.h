# ifndef MSC_SOCKET_HANDLER_H
# define MSC_SOCKET_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_socket

class Socket: public Handler {
public:
    Socket() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Socket();
    };
};

REGISTER_HANDLER(Socket, (int) TYPE, Socket::create);

#endif