#include "Mmap.h"

std::vector<argument_type> Mmap::getArgumentTypes() {
    return std::vector<argument_type>({AT_VOID_ADDR, AT_SIZE_T, AT_INT, AT_INT, AT_INT, AT_OFF_T});
}

argument_type Mmap::getReturnType() {
    return AT_PASLR_FOLLOW;
}