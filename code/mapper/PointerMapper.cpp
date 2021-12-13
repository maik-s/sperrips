#include "PointerMapper.h"
#include <cassert>

PointerMapper::PointerMapper() {
    the_map = {};
}

bool PointerMapper::map(void* recorded, void* replayed) {
    if (this->mappingExists(recorded)) {
        void* mapped = this->getMapping(recorded);
        return mapped == replayed;
    } else {
        the_map[recorded] = replayed;
        return true;
    }
}

void* PointerMapper::getMapping(void* recorded) {
    assert(this->mappingExists(recorded));
    return this->the_map[recorded];
}

bool PointerMapper::mappingExists(void* recorded) {
    return the_map.find(recorded) != the_map.end();
}