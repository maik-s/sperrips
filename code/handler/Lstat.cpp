#include "Lstat.h"

std::vector<argument_type> Lstat::getArgumentTypes() {
    return std::vector<argument_type>({AT_CHAR_PTR, AT_STRUCT_STAT});
}