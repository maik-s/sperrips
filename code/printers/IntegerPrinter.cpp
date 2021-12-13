#include "IntegerPrinter.h"

void IntegerPrinter::print(argument_value value) {
    if (value.type == AT_SIGNED_INT) {
        signed long val = value.int64;
        std::cout << val;

        if (val < 0) {
            std::cout << " (" << strerror(val * (-1)) << ")";
        }

        std::cout << std::endl;
    } else if (value.type == AT_SOCKLEN_T_PTR) {
        socklen_t rval = *(socklen_t*) value.str.c_str();
        printf("%d\n", rval);
    } else if (value.type == AT_UINT64_PTR) {
        uint64_t* rval = (uint64_t*) value.str.c_str();
        printf("%ld\n", *rval);
    } else {
        std::cout << value.int64 << std::endl;
    }
}