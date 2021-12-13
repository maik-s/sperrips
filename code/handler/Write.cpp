#include "Write.h"

std::vector<argument_type> Write::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_CHAR_PTR, AT_SIZE_T});
}