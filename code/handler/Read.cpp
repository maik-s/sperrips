#include "Read.h"

std::vector<argument_type> Read::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_OUT_BUFFER, AT_SIZE_T});
}