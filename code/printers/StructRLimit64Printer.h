# ifndef MSC_STRUCTRLIMIT64PRINTER_PRINTER_H
# define MSC_STRUCTRLIMIT64PRINTER_PRINTER_H

// automatically generated by addPrinter.py

#include "PrinterRegistry.h"
#include <sys/resource.h> // struct rlimit64

class StructRLimit64Printer: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructRLimit64Printer();
    };

};

REGISTER_PRINTER(StructRLimit64Printer, AT_STRUCT_RLIMIT64, StructRLimit64Printer::create);

#endif