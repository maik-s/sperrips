# ifndef MSC_POINTER_PRINTER_H
# define MSC_POINTER_PRINTER_H

#include "PrinterRegistry.h"

class PointerPrinter: public Printer {

public:
    void print(argument_value value);

    static Printer* create() {
        return new PointerPrinter();
    };

};

REGISTER_PRINTER(PointerPrinter, AT_VOID_ADDR, PointerPrinter::create);
REGISTER_PRINTER(SigSetTPtrPrinter, AT_SIGSET_T_PTR, PointerPrinter::create);

#endif