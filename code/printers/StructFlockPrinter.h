# ifndef MSC_STRUCT_FLOCK_PRINTER_H
# define MSC_STRUCT_FLOCK_PRINTER_H

#include "PrinterRegistry.h"
#include <fcntl.h> // struct flock, f_owner_ex

class StructFlockPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructFlockPrinter();
    };

};

REGISTER_PRINTER(StructFlockPrinter, AT_STRUCT_FLOCK, StructFlockPrinter::create);
REGISTER_PRINTER(StructFOwnerExPrinter, AT_STRUCT_F_OWNER_EX, StructFlockPrinter::create);

#endif