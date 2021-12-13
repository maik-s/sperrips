# ifndef MSC_STRUCT_TIMERSPEC_PRINTER_H
# define MSC_STRUCT_TIMERSPEC_PRINTER_H

#include "PrinterRegistry.h"
#include <sys/time.h> // struct timespec, setitimerval

class StructTimespecPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructTimespecPrinter();
    };

};

REGISTER_PRINTER(StructTimespecPrinter, AT_STRUCT_TIMESPEC, StructTimespecPrinter::create);
REGISTER_PRINTER(StructSetTimeInterval, AT_STRUCT_SET_ITIMERVAL, StructTimespecPrinter::create);
REGISTER_PRINTER(StructTimeValPrinter, AT_STRUCT_TIMEVAL, StructTimespecPrinter::create);
REGISTER_PRINTER(StructTimeZonePrinter, AT_STRUCT_TIMEZONE, StructTimespecPrinter::create);

#endif