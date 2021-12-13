#include "Pread64.h"

std::vector<argument_type> Pread64::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_OUT_BUFFER, AT_SIZE_T, AT_OFF_T});
}