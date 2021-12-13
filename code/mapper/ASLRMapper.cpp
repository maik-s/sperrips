#include "ASLRMapper.h"
#include <cassert>

ASLRMapper::ASLRMapper() {
    map_recorded = PointerMapper();
    map_replayed = PointerMapper();
}

/**
 * @brief Maps a given recorded value to a replayed value and vice versa.
 * It first checks whether the recorded value already has a mapping, further it checks
 * whether this mapping matches the replayed value.
 * If no mapping for the recorded exists, it checks whether the replayed value
 * as mapping and if this different than the recorded one.
 *
 * @param recorded Recorded value to map.
 * @param replayed Replayed value to map.
 * @return true If mapping of \p recorded is equal to \p replayed or if a new mapping has been made.
 * @return false If existing mappings do not match.
 */

bool ASLRMapper::map(void* recorded, void* replayed) {
    if (map_recorded.mappingExists(recorded)) {
        void* mapped = this->getMapping(recorded);
        printf("Mapping exists %p (-> %p)\n", recorded, this->getMapping(recorded));
        return mapped == replayed;
    } else {
        if (map_replayed.mappingExists(replayed) && map_replayed.getMapping(replayed) != recorded) {
            fprintf(stderr, "Pointer misuse identified! Expected %p, but got %p.\n", map_replayed.getMapping(replayed), recorded);
            return false;
        }

        printf("Mapping %p -> %p\n", recorded, replayed);
        map_recorded.map(recorded, replayed);
        map_replayed.map(replayed, recorded);
        return true;
    }
}

/**
 * @brief Returns the mapping for a \p recorded key.
 *
 * @param recorded Mapping key.
 * @return void* Mapped value for key \p recorded.
 */

void* ASLRMapper::getMapping(void* recorded) {
    assert(this->mappingExists(recorded));
    return map_recorded.getMap()[recorded];
}

/**
 * @brief Checks whether \p recorded has a mapping.
 *
 * @param recorded Mapping key.
 * @return true \p recorded has a mapping.
 * @return false \p recorded does not have a mapping.
 */

bool ASLRMapper::mappingExists(void* recorded) {
    return map_recorded.getMap().find(recorded) != map_recorded.getMap().end();
}