# ifndef MSC_STRUCT_STAT_PRINTER_H
# define MSC_STRUCT_STAT_PRINTER_H

#include "PrinterRegistry.h"
#include <sys/stat.h> //struct stat

class StructStatPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructStatPrinter();
    };

};

REGISTER_PRINTER(StructStatPrinter, AT_STRUCT_STAT, StructStatPrinter::create);

#endif