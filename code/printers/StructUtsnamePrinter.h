# ifndef MSC_STRUCT_UTSNAME_PRINTER_H
# define MSC_STRUCT_UTSNAME_PRINTER_H

#include "PrinterRegistry.h"
#include <sys/utsname.h> // struct utsname

class StructUtsnamePrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructUtsnamePrinter();
    };

};

REGISTER_PRINTER(StructUtsnamePrinter, AT_STRUCT_UTSNAME, StructUtsnamePrinter::create);

#endif