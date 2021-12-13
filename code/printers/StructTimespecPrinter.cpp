#include "StructTimespecPrinter.h"

void StructTimespecPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    if (value.type == AT_STRUCT_TIMESPEC) {
        struct timespec* data = (struct timespec*) value.str.c_str();
        printf("  tv_sec: %ld\n", data->tv_sec);
        printf("  tv_nsec: %ld\n", data->tv_nsec);

    } else if (value.type == AT_STRUCT_SET_ITIMERVAL) {
        // struct itimerval {
        //     struct timeval it_interval; /* next value */
        //     struct timeval it_value;    /* current value */
        // };
        struct itimerval* data = (struct itimerval*) value.str.c_str();
        printf("  it_interval: %p\n", (void*)&data->it_interval);
        printf("  it_value: %p\n", (void*)&data->it_value);
    } else if (value.type == AT_STRUCT_TIMEVAL) {
        // struct timeval {
        //     time_t      tv_sec;         /* seconds */
        //     suseconds_t tv_usec;        /* microseconds */
        // };
        struct timeval* data = (struct timeval*) value.str.c_str();
        printf("  tv_sec: %p\n", (void*)&data->tv_sec);
        printf("  tv_usec: %p\n", (void*)&data->tv_usec);
    } else if (value.type == AT_STRUCT_TIMEZONE) {
        struct timezone* data = (struct timezone*) value.str.c_str();
        printf("  tz_minuteswest: %p\n", (void*)&data->tz_minuteswest);
        printf("  tz_dsttime: %p\n", (void*)&data->tz_dsttime);
    }
}

int StructTimespecPrinter::getStructLength(argument_type type) {
    if (type == AT_STRUCT_TIMESPEC) {
        return sizeof(timespec);
    } else if (type == AT_STRUCT_SET_ITIMERVAL) {
        return sizeof(itimerval);
    } else if (type == AT_STRUCT_TIMEVAL) {
        return sizeof(timeval);
    } else if (type == AT_STRUCT_TIMEZONE) {
        return sizeof(timezone);
    }  else {
        return -1;
    }
}