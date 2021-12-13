#include "Mprotect.h"

std::vector<argument_type> Mprotect::getArgumentTypes() {
    return std::vector<argument_type>({AT_VOID_ADDR, AT_SIZE_T, AT_INT});
}