#include "StructPollfdPrinter.h"

void StructPollfdPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct pollfd* data = (struct pollfd*) value.str.c_str();

    // struct pollfd {
    //     int   fd;         /* file descriptor */
    //     short events;     /* requested events */
    //     short revents;    /* returned events */
    // };

    printf("  fd: %d\n", data->fd);

    printf("  events: %d\n", data->events);

    printf("  revents: %d\n", data->revents);
}

int StructPollfdPrinter::getStructLength(argument_type type) {
    return sizeof(pollfd);
}