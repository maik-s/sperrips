#include "GetTimeOfDay.h"
// automatically generated by addhandler.py

std::vector<argument_type> GetTimeOfDay::getArgumentTypes() {
    return std::vector<argument_type>({AT_STRUCT_TIMEVAL, AT_STRUCT_TIMEZONE});
}