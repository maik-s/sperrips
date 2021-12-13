# ifndef MSC_SET_TID_ADDRESS_HANDLER_H
# define MSC_SET_TID_ADDRESS_HANDLER_H

#include "HandlerRegistry.h"

#define TYPE SYS_set_tid_address

class SetTidAddress: public Handler {
public:
    SetTidAddress() {
        _type = TYPE;
    }

    std::vector<argument_type> getArgumentTypes();

    static Handler* create() {
        return new SetTidAddress();
    };
};

REGISTER_HANDLER(SetTidAddress, (int) TYPE, SetTidAddress::create);

#endif