# ifndef MSC_HANDLER_REGISTRY_H
# define MSC_HANDLER_REGISTRY_H

#include <string>
#include <unordered_map>
#include <functional>
#include "Handler.h"
#include <cassert>

#define REGISTER_HANDLER(handler_name, handler_type, create_func) \
    bool handler_name ## _entry = HandlerRegistry<Handler>::add(handler_type, (create_func))

template <typename T>
class HandlerRegistry {
public:
    typedef std::function<T*()> FactoryFunction;
    typedef std::unordered_map<int, FactoryFunction> FactoryMap;

    static bool add(int type, FactoryFunction fac) {
        auto map = getFactoryMap();

        if (map.find(type) != map.end()) {
            assert(false); // fail when try to insert element, even though its already inserted
            return false;
        }

        getFactoryMap()[type] = fac;
        return true;
    }

    static T* create(int type) {
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