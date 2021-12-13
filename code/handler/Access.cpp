#include "Access.h"

std::vector<argument_type> Access::getArgumentTypes() {
    return std::vector<argument_type>({AT_CHAR_PTR, AT_INT});
};