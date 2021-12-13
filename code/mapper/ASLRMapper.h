# ifndef MSC_ASLR_MAPPER_H
# define MSC_ASLR_MAPPER_H

#include "ValueMapper.h"
#include "PointerMapper.h"
#include <cstdio>
#include <memory> // unique_ptr


class ASLRMapper: public ValueMapper<void*> {
private:
    int verbosityLevel = 0;
    PointerMapper map_recorded;
    PointerMapper map_replayed;
public:
    ASLRMapper();
    bool map(void* recorded, void* replayed);
    void* getMapping(void* ptr);
    bool mappingExists(void* ptr);
};
#endif
