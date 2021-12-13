#include "Ioctl.h"

std::vector<argument_type> Ioctl::getArgumentTypes() {
    return std::vector<argument_type>({AT_INT, AT_INT, AT_VOID_ADDR});
}