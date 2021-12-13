#include "ReadLink.h"

std::vector<argument_type> ReadLink::getArgumentTypes() {
    return std::vector<argument_type>({AT_CHAR_PTR, AT_OUT_BUFFER, AT_SIZE_T});
}