#include "Fchmod.h"

std::vector<argument_type> FChmod::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_MODE_T});
}