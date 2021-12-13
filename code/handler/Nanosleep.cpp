#include "Nanosleep.h"

std::vector<argument_type> Nanosleep::getArgumentTypes() {
    return std::vector<argument_type>({AT_STRUCT_TIMESPEC, AT_STRUCT_TIMESPEC});
}