#include "Fadvise64.h"

std::vector<argument_type> Fadvise64::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_OFF_T, AT_OFF_T, AT_INT});
}