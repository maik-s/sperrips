# ifndef MSC_PRINTER_REGISTRY_H
# define MSC_PRINTER_REGISTRY_H

#include <string>
#include <unordered_map>
#include <functional>
#include "Printer.h"
#include "../TypeDefs.h"

#define REGISTER_PRINTER(printer_name, printer_type, create_func) \
    bool printer_name ## _entry = PrinterRegistry<Printer>::add(printer_type, (create_func))

template <typename T>
class PrinterRegistry {
public:
    typedef std::function<T*()> FactoryFunction;
    typedef std::unordered_map<int, FactoryFunction> FactoryMap;

    static bool add(argument_type type, FactoryFunction fac) {
        auto map = getFactoryMap();

        if (map.find(type) != map.end()) {
            assert(false); // fail when try to insert element, even though its already inserted
            return false;
        }

        getFactoryMap()[type] = fac;
        return true;
    }

    static T* create(argument_type type) {
        auto map = getFactoryMap();

        if (map.find(type) == map.end()) {
            return nullptr;
        }

        return map[type]();
    }

private:
    // Use Meyer's singleton to prevent SIOF
    static FactoryMap& getFactoryMap() {
        static FactoryMap map;
        return map;
    }
};

#endif