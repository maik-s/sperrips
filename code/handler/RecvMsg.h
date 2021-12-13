# ifndef MSC_RECVMSG_HANDLER_H
# define MSC_RECVMSG_HANDLER_H

#include <sys/socket.h> // struct sockaddr, msghdr
#include "HandlerRegistry.h"

#define TYPE SYS_recvmsg

class RecvMsg: public Handler {
public:
    RecvMsg() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new RecvMsg();
    };
};

REGISTER_HANDLER(RecvMsg, (int) TYPE, RecvMsg::create);

#endif