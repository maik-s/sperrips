# ifndef MSC_SENDTO_HANDLER_H
# define MSC_SENDTO_HANDLER_H

#include <sys/socket.h> // struct sockaddr, msghdr
#include "HandlerRegistry.h"

#define TYPE SYS_sendto

class SendTo: public Handler {
public:
    SendTo() {
        _type = TYPE;
    }
    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SendTo();
    };
};

REGISTER_HANDLER(SendTo, (int) TYPE, SendTo::create);

#endif