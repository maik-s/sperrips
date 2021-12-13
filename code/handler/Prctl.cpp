#include "Prctl.h"

std::vector<argument_type> Prctl::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_LONG, AT_VOID_ADDR, AT_VOID_ADDR, AT_LONG});
}