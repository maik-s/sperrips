#include "StructCapUserPrinter.h"

void StructCapUserPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    if (value.type == AT_STRUCT_CAP_USER_DATA_T) {
        struct __user_cap_data_struct* data = (struct __user_cap_data_struct*) value.str.c_str();
        printf("  effective: %d\n", data->effective);
        printf("  permitted: %d\n", data->permitted);
        printf("  inheritable: %d\n", data->inheritable);
    } else if (value.type == AT_STRUCT_CAP_USER_HEADER_T) {
        struct __user_cap_header_struct* data = (struct __user_cap_header_struct*) value.str.c_str();
        printf("  version: %d\n", data->version);
        printf("  pid: %d\n", data->pid);
    }
}

int StructCapUserPrinter::getStructLength(argument_type type) {
    if (type == AT_STRUCT_CAP_USER_DATA_T) {
        return sizeof(__user_cap_data_struct);
    } else if (type == AT_STRUCT_CAP_USER_HEADER_T) {
        return sizeof(__user_cap_header_struct);
    } else {
        return -1;
    }
}