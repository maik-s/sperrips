#include "Uname.h"

std::vector<argument_type> Uname::getArgumentTypes() {
    return std::vector<argument_type>({AT_STRUCT_UTSNAME});
}