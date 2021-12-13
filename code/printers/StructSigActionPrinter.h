# ifndef MSC_STRUCT_SIGACTION_PRINTER_H
# define MSC_STRUCT_SIGACTION_PRINTER_H

#include "PrinterRegistry.h"
#include <signal.h> // struct sigaction
#include <cstring> // memcopy

class StructSigActionPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    std::string updateStructAdresses(argument_type type, std::string recorded, std::string replayed, ASLRMapper* pointerMapper);

    static Printer* create() {
        return new StructSigActionPrinter();
    };

};

REGISTER_PRINTER(StructSigActionPrinter, AT_STRUCT_SIGACTION, StructSigActionPrinter::create);

#endif