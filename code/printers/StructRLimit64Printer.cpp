#include "StructRLimit64Printer.h"

void StructRLimit64Printer::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct rlimit64* data = (struct rlimit64*) value.str.c_str();

    // /* The current (soft) limit.  */
    // rlim64_t rlim_cur;
    // /* The hard limit.  */
    // rlim64_t rlim_max;
    printf("  rlim_cur: %ld\n", data->rlim_cur);

    printf("  rlim_max: %ld\n", data->rlim_max);
}

int StructRLimit64Printer::getStructLength(argument_type type) {
    return sizeof(struct rlimit64);
}
