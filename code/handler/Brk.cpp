#include "Brk.h"


std::vector<argument_type> Brk::getArgumentTypes() {
    return std::vector<argument_type>({AT_VOID_ADDR});
}

argument_type Brk::getReturnType() {
    return AT_VOID_ADDR;
}