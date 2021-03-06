# ifndef MSC_PRLIMIT64_HANDLER_H
# define MSC_PRLIMIT64_HANDLER_H
// automatically generated by addhandler.py

#include "HandlerRegistry.h"

#define TYPE SYS_prlimit64

class PrLimit64: public Handler {
public:
    PrLimit64() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new PrLimit64();
    };
};

REGISTER_HANDLER(PrLimit64, (int) TYPE, PrLimit64::create);

#endif