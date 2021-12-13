#include "RtSigaction.h"

std::vector<argument_type> RtSigaction::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_STRUCT_SIGACTION, AT_STRUCT_SIGACTION, AT_SIZE_T});
}