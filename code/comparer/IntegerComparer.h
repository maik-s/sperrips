# ifndef MSC_INTEGER_COMPARER_H
# define MSC_INTEGER_COMPARER_H

#include "ArgumentComparer.h"
#include <cstdint> // unint64_t

class IntegerComparer: public ArgumentComparer<uint64_t> {
private:
    int verbosityLevel = 0;
public:
    bool compare(uint64_t recorded, uint64_t replayed);
};
#endif