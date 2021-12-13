# ifndef MSC_PRINTER_H
# define MSC_PRINTER_H

#include <string>
#include <iostream>
#include "../TypeDefs.h"
#include <vector>
#include "../mapper/ASLRMapper.h"

/**
 * @brief Base class for printer classes.
 *
 */
class Printer {
public:

    /**
     * @brief Pretty print \p argument.
     * It implements custom prinint logic for the particular type, e.g., integer or various structs.
     *
     * @param argument Value to print.
     */
    virtual void print(argument_value argument) {};

    /**
     * @brief Returns the size of a struct, indicated by \p type.
     *
     * @param type Given argument type.
     * @return int size of the struct as on this platform (`sizeof(..)`)
     */

    virtual int getStructLength(argument_type type) {
        std::cerr << "[Error]: getStructLength(argument_type type) shouldnt be called on non-struct printer!" << std::endl;
        return -1;
    }

    /**
     * @brief Logic to update addresses of nested structures.
     *
     * @param type Indicates the struct's type.
     * @param recorded Byte stream as retrieved in the recording phase.
     * @param replayed Byte stream as retrieved in the replaying phase.
     * @param pointerMapper Reference to pointerMapper instance.
     * @return std::string Bytes stream of the updated struct.
     */

    virtual std::string updateStructAdresses(argument_type type, std::string recorded, std::string replayed, ASLRMapper* pointerMapper) {
        std::cerr << "[Error]: updateStructAdresses() shouldnt be called on non-struct printer!" << std::endl;
        return std::string();
    }

    static Printer* create() {
        return new Printer();
    }
};

#endif