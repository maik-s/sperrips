#include "StructUtsnamePrinter.h"

void StructUtsnamePrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct utsname* data = (struct utsname*) value.str.c_str();

    // struct utsname {
    //     char sysname[];    /* Operating system name (e.g., "Linux") */
    //     char nodename[];   /* Name within "some implementation-defined
    //                             network" */
    //     char release[];    /* Operating system release
    //                             (e.g., "2.6.28") */
    //     char version[];    /* Operating system version */
    //     char machine[];    /* Hardware identifier */
    // #ifdef _GNU_SOURCE
    //     char domainname[]; /* NIS or YP domain name */
    // #endif
    // };

    printf("  sysname: %s\n", data->sysname);

    printf("  nodename: %s\n", data->nodename);

    printf("  release: %s\n", data->release);

    printf("  version: %s\n", data->version);

    printf("  machine: %s\n", data->machine);

    printf("  domainname: %s\n", data->domainname);
}

int StructUtsnamePrinter::getStructLength(argument_type type) {
    return sizeof(utsname);
}