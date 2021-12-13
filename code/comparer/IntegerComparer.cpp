#include "ArgumentComparer.h"
#include "IntegerComparer.h"

bool IntegerComparer::compare(uint64_t recorded, uint64_t replayed) {
    return recorded == replayed;
}