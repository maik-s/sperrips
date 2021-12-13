#include "PointerPrinter.h"

void PointerPrinter::print(argument_value value) {
    printf("0x%08lX\n", (long unsigned int)value.ptr);
}