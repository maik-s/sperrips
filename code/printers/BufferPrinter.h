# ifndef MSC_BUFFER_PRINTER_H
# define MSC_BUFFER_PRINTER_H

#include "PrinterRegistry.h"
#include "../Utils.h"
#include <algorithm>    // std::min

class BufferPrinter: public Printer {

public:
    void print(argument_value value);

    static Printer* create() {
        return new BufferPrinter();
    };

};

REGISTER_PRINTER(BufferPrinter, AT_OUT_BUFFER, BufferPrinter::create);
REGISTER_PRINTER(VoidPointerFollowPrinter, AT_VOID_PTR_FOLLOW, BufferPrinter::create);
REGISTER_PRINTER(PASLRFollowPrinter, AT_PASLR_FOLLOW, BufferPrinter::create);

#endif