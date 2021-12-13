# ifndef MSC_INTEGER_PRINTER_H
# define MSC_INTEGER_PRINTER_H

#include "PrinterRegistry.h"
#include <string.h> // for strerror
#include <sys/socket.h> // for socklen_t

class IntegerPrinter: public Printer {

public:
    void print(argument_value value);

    static Printer* create() {
        return new IntegerPrinter();
    };

};

REGISTER_PRINTER(IntegerPrinter, AT_INT, IntegerPrinter::create);
REGISTER_PRINTER(SignedIntegerPrinter, AT_SIGNED_INT, IntegerPrinter::create);
REGISTER_PRINTER(LongPrinter, AT_LONG, IntegerPrinter::create);
REGISTER_PRINTER(SizeTPrinter, AT_SIZE_T, IntegerPrinter::create);
REGISTER_PRINTER(OffTPrinter, AT_OFF_T, IntegerPrinter::create);
REGISTER_PRINTER(ModeTPrinter, AT_MODE_T, IntegerPrinter::create);
REGISTER_PRINTER(UidTPrinter, AT_UID_T, IntegerPrinter::create);
REGISTER_PRINTER(SigsetTPrinter, AT_SIGSET_T, IntegerPrinter::create);
REGISTER_PRINTER(SockLenTPrinter, AT_SOCKLEN_T, IntegerPrinter::create);
REGISTER_PRINTER(IgnorePrinter, AT_IGNORE, IntegerPrinter::create);
REGISTER_PRINTER(SocklenTPtrPrinter, AT_SOCKLEN_T_PTR, IntegerPrinter::create);
REGISTER_PRINTER(PidTPrinter, AT_PID_T, IntegerPrinter::create);
REGISTER_PRINTER(Uint64PointerPrinter, AT_UINT64_PTR, IntegerPrinter::create);

#endif