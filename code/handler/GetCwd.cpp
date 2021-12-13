#include "GetCwd.h"

std::vector<argument_type> GetCwd::getArgumentTypes() {
    return std::vector<argument_type>({AT_OUT_BUFFER, AT_LONG});
}