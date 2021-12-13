#include "StructSysinfoPrinter.h"

void StructSysinfoPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct sysinfo* data = (struct sysinfo*) value.str.c_str();

    // struct sysinfo {
    //     long uptime;             /* Seconds since boot */
    //     unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    //     unsigned long totalram;  /* Total usable main memory size */
    //     unsigned long freeram;   /* Available memory size */
    //     unsigned long sharedram; /* Amount of shared memory */
    //     unsigned long bufferram; /* Memory used by buffers */
    //     unsigned long totalswap; /* Total swap space size */
    //     unsigned long freeswap;  /* Swap space still available */
    //     unsigned short procs;    /* Number of current processes */
    //     unsigned long totalhigh; /* Total high memory size */
    //     unsigned long freehigh;  /* Available high memory size */
    //     unsigned int mem_unit;   /* Memory unit size in bytes */
    //     char _f[20-2*sizeof(long)-sizeof(int)];
    //                             /* Padding to 64 bytes */
    // };

    printf("  uptime: %ld\n", data->uptime);

    printf("  loads: %ln\n", data->loads);

    printf("  totalram: %ld\n", data->totalram);

    printf("  freeram: %ld\n", data->freeram);

    printf("  sharedram: %ld\n", data->sharedram);

    printf("  bufferram: %ld\n", data->bufferram);

    printf("  totalswap: %ld\n", data->totalswap);

    printf("  freeswap: %ld\n", data->freeswap);

    printf("  procs: %d\n", data->procs);

    printf("  totalhigh: %ld\n", data->totalhigh);

    printf("  freehigh: %ld\n", data->freehigh);

    printf("  mem_unit: %d\n", data->mem_unit);
}

int StructSysinfoPrinter::getStructLength(argument_type type) {
    return sizeof(struct sysinfo);
}