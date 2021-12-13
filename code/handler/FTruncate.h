# ifndef MSC_FTRUNCATE_HANDLER_H
# define MSC_FTRUNCATE_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_ftruncate

class FTruncate: public Handler {
public:
    FTruncate() {
        _type = TYPE;
    };

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new FTruncate();
    };
};

REGISTER_HANDLER(FTruncate, (int) TYPE, FTruncate::create);

#endif