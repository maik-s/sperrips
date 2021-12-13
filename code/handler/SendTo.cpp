#include "SendTo.h"

std::vector<argument_type> SendTo::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_VOID_PTR_FOLLOW, AT_SIZE_T, AT_INT, AT_STRUCT_SOCKADDR, AT_SOCKLEN_T});
}