#include "GetSockName.h"

std::vector<argument_type> GetSockName::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_STRUCT_SOCKADDR, AT_SOCKLEN_T_PTR});
}