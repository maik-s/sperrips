#include "ArchPrctl.h"
#include <asm/prctl.h>

std::vector<argument_type> ArchPrctl::getArgumentTypes() {
    if (_argumentTypes.empty()) {
        _argumentTypes = std::vector<argument_type>({AT_INT, AT_VOID_ADDR}); // actuall the second param is AT_INT, but we need it to be AT_VOID_ADDR for ASLR support
    }

    return _argumentTypes;
}

void ArchPrctl::rewriteArgumentType(int argumentIndex, argument_value argumentValue) {
    if (argumentIndex != 0) {
        return;
    }

    bool rewritten = false;

    if (argumentValue.int64 == ARCH_GET_CPUID  || argumentValue.int64 == ARCH_GET_FS || argumentValue.int64 == ARCH_GET_GS) {
        _argumentTypes.at(1) = AT_UINT64_PTR;
        rewritten = true;
    } else if (argumentValue.int64 == ARCH_SET_CPUID || argumentValue.int64 == ARCH_SET_FS || argumentValue.int64 == ARCH_SET_GS) {
        // do nothing
        rewritten = true;
        _argumentTypes.at(1) = AT_VOID_ADDR; // actuall it's long, but we need it to be AT_VOID_ADDR for ASLR support
    }

    if (rewritten) {
        printf("[i] Argument %d rewritten to type %s\n", 1, ((std::string)magic_enum::enum_name(_argumentTypes.at(1))).c_str());
    }
}