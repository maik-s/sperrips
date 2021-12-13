#include "FTruncate.h"

std::vector<argument_type> FTruncate::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_OFF_T});
}