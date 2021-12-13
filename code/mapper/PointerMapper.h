# ifndef MSC_POINTER_MAPPER_H
# define MSC_POINTER_MAPPER_H

#include "ValueMapper.h"
#include <cstdio>
class PointerMapper: public ValueMapper<void*> {
private:
    int verbosityLevel = 0;
    std::map<void*, void*> the_map;
public:
    PointerMapper();
    bool map(void* recorded, void* replayed);
    void* getMapping(void* ptr);
    bool mappingExists(void* ptr);
    std::map<void*, void*> getMap() {
        return the_map;
    };
};
#endif
