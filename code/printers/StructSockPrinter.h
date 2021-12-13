# ifndef MSC_STRUCT_SOCK_PRINTER_H
# define MSC_STRUCT_SOCK_PRINTER_H

#include "PrinterRegistry.h"
#include <sys/socket.h> // struct sockaddr, msghdr
#include <netinet/in.h> // sockaddrin
#include "../Utils.h"
#include <arpa/inet.h> // inet_ntop
#include <sys/un.h> // sockaddr_un

class StructSockPrinter: public Printer {

public:
    void print(argument_value value);

    int getStructLength(argument_type type);

    std::string updateStructAdresses(argument_type type, std::string recorded, std::string replayed, ASLRMapper* pointerMapper);

    static Printer* create() {
        return new StructSockPrinter();
    };

};

REGISTER_PRINTER(StructSockPrinter, AT_STRUCT_SOCKADDR, StructSockPrinter::create);
REGISTER_PRINTER(StructMsgheaderPrinter, AT_STRUCT_MSGHDR, StructSockPrinter::create);

#endif