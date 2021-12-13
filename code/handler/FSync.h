# ifndef MSC_FSYNC_HANDLER_H
# define MSC_FSYNC_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_fsync

class FSync: public Handler {
public:
    FSync() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new FSync();
    };
};

REGISTER_HANDLER(FSync, (int) TYPE, FSync::create);

#endif