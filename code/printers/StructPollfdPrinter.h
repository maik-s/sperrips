# ifndef MSC_STRUCT_POLLFD_PRINTER_H
# define MSC_STRUCT_POLLFD_PRINTER_H

#include "PrinterRegistry.h"
#include <poll.h> // struct pollfd

class StructPollfdPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    static Printer* create() {
        return new StructPollfdPrinter();
    };

};

REGISTER_PRINTER(StructPollfdPrinter, AT_STRUCT_POLLFD, StructPollfdPrinter::create);

#endif