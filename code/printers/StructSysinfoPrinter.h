# ifndef MSC_STRUCT_SYSINFO_PRINTER_H
# define MSC_STRUCT_SYSINFO_PRINTER_H

#include "PrinterRegistry.h"
#include <sys/sysinfo.h> // struct sysinfo

class StructSysinfoPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructSysinfoPrinter();
    };

};

REGISTER_PRINTER(StructSysinfoPrinter, AT_STRUCT_SYSINFO, StructSysinfoPrinter::create);

#endif