#include "SigProcMask.h"

std::vector<argument_type> SigProcMask::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_SIGSET_T_PTR, AT_SIGSET_T_PTR, AT_SIZE_T});
}