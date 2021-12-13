#include "SetItimer.h"

std::vector<argument_type> SetItimer::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_STRUCT_SET_ITIMERVAL, AT_STRUCT_SET_ITIMERVAL});
}