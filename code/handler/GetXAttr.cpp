#include "GetXAttr.h"

std::vector<argument_type> GetXAttr::getArgumentTypes() {
    return std::vector<argument_type>({AT_CHAR_PTR, AT_CHAR_PTR, AT_VOID_ADDR, AT_SIZE_T});
}