# ifndef MSC_STRUCT_CAP_USER_PRINTER_H
# define MSC_STRUCT_CAP_USER_PRINTER_H

#include "PrinterRegistry.h"
#include <linux/capability.h> // for structs cap_user_header t and cap_user_data_t

class StructCapUserPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructCapUserPrinter();
    };

};

REGISTER_PRINTER(StructCapUserDataPrinter, AT_STRUCT_CAP_USER_DATA_T, StructCapUserPrinter::create);
REGISTER_PRINTER(StructCapUserHeaderPrinter, AT_STRUCT_CAP_USER_HEADER_T, StructCapUserPrinter::create);

#endif