#include "ArgumentComparer.h"
#include "BufferComparer.h"

bool BufferComparer::compare(std::string recorded, std::string replayed) {
    return recorded.compare(replayed) == 0;
}