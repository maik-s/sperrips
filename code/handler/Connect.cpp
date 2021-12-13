#include "Connect.h"

std::vector<argument_type> Connect::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_STRUCT_SOCKADDR, AT_INT});
}