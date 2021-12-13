#include "OpenAt.h"

std::vector<argument_type> OpenAt::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_CHAR_PTR, AT_INT, AT_MODE_T});
}