#include "Stat.h"

std::vector<argument_type> Stat::getArgumentTypes() {
    return std::vector<argument_type>({AT_CHAR_PTR, AT_STRUCT_STAT});
}