# ifndef MSC_POLL_HANDLER_H
# define MSC_POLL_HANDLER_H

#include <poll.h> // struct pollfd
#include "HandlerRegistry.h"

#define TYPE SYS_poll

class Poll: public Handler {
public:
    Poll() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new Poll();
    };
};

REGISTER_HANDLER(Poll, (int) TYPE, Poll::create);

#endif