# ifndef MSC_STRING_PRINTER_H
# define MSC_STRING_PRINTER_H

#include "PrinterRegistry.h"

class StringPrinter: public Printer {

public:
    void print(argument_value value);

    static Printer* create() {
        return new StringPrinter();
    };

};

REGISTER_PRINTER(StringPrinter, AT_CHAR_PTR, StringPrinter::create);

#endif