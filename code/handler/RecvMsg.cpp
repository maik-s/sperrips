#include "RecvMsg.h"

std::vector<argument_type> RecvMsg::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_STRUCT_MSGHDR, AT_INT});
}