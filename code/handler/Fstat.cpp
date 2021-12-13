#include "Fstat.h"

std::vector<argument_type> Fstat::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_STRUCT_STAT});
}