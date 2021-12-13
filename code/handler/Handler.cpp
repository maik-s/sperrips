#include "Handler.h"

/**
 * @brief Prints the arguments and return value of the handler to the stdout.
 *
 */

void Handler::print() {
    std::unique_ptr<Printer> printer;

    for (int i = 0; i < _argumentValues.size(); i++) {
        argument_value val = _argumentValues.at(i);
        auto arg_name = magic_enum::enum_name(val.type);
        printer = std::unique_ptr<Printer>(PrinterRegistry<Printer>::create(val.type));
        auto repeated_space = "" ;
        std::cout << i << " " << arg_name << "" << repeated_space << ": ";

        if (printer.get() == nullptr) {
            std::cout << "no printer found" << std::endl;
        } else {
            printer->print(val);
        }
    }

    std::cout << std::endl;

    std::cout << "Return value (" << (std::string)(magic_enum::enum_name(getReturnType())) << "): ";
    printer = std::unique_ptr<Printer>(PrinterRegistry<Printer>::create(getReturnType()));

    if (printer.get() == nullptr) {
        std::cout << "no printer found" << std::endl;
    } else {
        printer->print(getReturnValueAsArgument());
    }

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
}