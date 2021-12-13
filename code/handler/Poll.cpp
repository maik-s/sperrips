#include "Poll.h"

std::vector<argument_type> Poll::getArgumentTypes() {
    return std::vector<argument_type>({AT_STRUCT_POLLFD, AT_INT, AT_INT});
}