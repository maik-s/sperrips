#include "Capget.h"

std::vector<argument_type> Capget::getArgumentTypes() {
    return std::vector<argument_type>({AT_STRUCT_CAP_USER_HEADER_T, AT_STRUCT_CAP_USER_DATA_T});
}