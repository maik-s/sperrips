#include "GetSockOpt.h"

std::vector<argument_type> GetSockOpt::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_INT, AT_INT, AT_VOID_PTR_FOLLOW, AT_SOCKLEN_T_PTR});
}