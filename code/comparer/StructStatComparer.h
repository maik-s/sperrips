# ifndef MSC_STRUCT_STAT_H
# define MSC_STRUCT_STAT_H

#include "ArgumentComparer.h"
#include <string>
#include <sys/stat.h>
#include <iostream>

class StructStatComparer: public ArgumentComparer<struct stat*> {
private:
        int verbosityLevel = 0;
public:
        //StructStatComparer(InodeComparer, TimeComparer);
        bool compare(struct stat* recorded, struct stat* replayed);
        void logMessage(std::string message);
    };
#endif