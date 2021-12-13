# ifndef MSC_VALUE_MAPPER_H
# define MSC_VALUE_MAPPER_H
#include <map>
#include <cstdio>

template<class T>
/**
 * @brief Base class for mapper classes.
 */
class ValueMapper {
private:
    int verbosityLevel = 0;
public:
    virtual bool map(T recorded, T replayed) = 0;
    virtual T getMapping(T key) = 0;
};
#endif