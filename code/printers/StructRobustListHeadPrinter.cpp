#include "StructRobustListHeadPrinter.h"

void StructRobustListHeadPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct robust_list_head* data = (struct robust_list_head*) value.str.c_str();

    printf("  list: %p\n", &data->list);

    printf("  futex_offset: %ld\n", data->futex_offset);

    printf("  list_op_pending: %p\n", &data->list_op_pending);
}

int StructRobustListHeadPrinter::getStructLength(argument_type type) {
    return sizeof(struct robust_list_head);
}
