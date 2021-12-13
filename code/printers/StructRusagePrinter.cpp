#include "StructRusagePrinter.h"

void StructRusagePrinter::print(argument_value value) {
    //TODO: implementation
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct rusage* data = (struct rusage*) value.str.c_str();

    printf(" ru_utime: %p\n", &data->ru_utime);

    printf(" ru_stime: %p\n", &data->ru_stime);

    printf("  ru_maxrss: %ld\n", data->ru_maxrss);

    printf("  ru_ixrss: %ld\n", data->ru_ixrss);

    printf("  ru_idrss: %ld\n", data->ru_idrss);

    printf("  ru_isrss: %ld\n", data->ru_isrss);

    printf("  ru_minflt: %ld\n", data->ru_minflt);

    printf("  ru_majflt: %ld\n", data->ru_majflt);

    printf("  ru_nswap: %ld\n", data->ru_nswap);

    printf("  ru_inblock: %ld\n", data->ru_inblock);

    printf("  ru_oublock: %ld\n", data->ru_oublock);

    printf("  ru_msgsnd: %ld\n", data->ru_msgsnd);

    printf("  ru_msgrcv: %ld\n", data->ru_msgrcv);

    printf("  ru_nsignals: %ld\n", data->ru_nsignals);

    printf("  ru_nvcsw: %ld\n", data->ru_nvcsw);

    printf("  ru_nivcsw: %ld\n", data->ru_nivcsw);
}

int StructRusagePrinter::getStructLength(argument_type type) {
    std::cout << "StructRusagePrinter::getStructLength" << std::endl;
    return sizeof(struct rusage);
}