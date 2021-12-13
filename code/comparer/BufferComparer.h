# ifndef MSC_STRING_COMPARER_H
# define MSC_STRING_COMPARER_H

#include "ArgumentComparer.h"
#include <string>

class BufferComparer: public ArgumentComparer<std::string> {
private:
    int verbosityLevel = 0;
public:
    bool compare(std::string recorded, std::string replayed);
};
#endif