#include "BufferPrinter.h"

void BufferPrinter::print(argument_value value) {
    int bytes = std::min((int)value.str.size(), 120);

    if (value.type == AT_PASLR_FOLLOW) {
        printf("0x%08lX", (long unsigned int)value.ptr);
    }

    printf("\n  [i] Print %d of %ld bytes:\n", bytes, value.str.size());

    if (bytes > 0) Utils::dump(value.str.c_str(), bytes, "  ");
}