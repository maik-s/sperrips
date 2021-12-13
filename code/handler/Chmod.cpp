#include "Chmod.h"

std::vector<argument_type> Chmod::getArgumentTypes() {
    return std::vector<argument_type>({AT_CHAR_PTR, AT_MODE_T});
}