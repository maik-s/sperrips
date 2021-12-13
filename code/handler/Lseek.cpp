#include "Lseek.h"

std::vector<argument_type> Lseek::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_OFF_T, AT_INT});
}