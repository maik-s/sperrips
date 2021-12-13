#include "Sysinfo.h"

std::vector<argument_type> Sysinfo::getArgumentTypes() {
    return std::vector<argument_type>({AT_STRUCT_SYSINFO});
}