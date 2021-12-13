#include "StructFlockPrinter.h"

void StructFlockPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    if (value.type == AT_STRUCT_FLOCK) {
        struct flock* data = (struct flock*) value.str.c_str();
        // struct flock {
        //     ...
        //     short l_type;    /* Type of lock: F_RDLCK,
        //                         F_WRLCK, F_UNLCK */
        //     short l_whence;  /* How to interpret l_start:
        //                         SEEK_SET, SEEK_CUR, SEEK_END */
        //     off_t l_start;   /* Starting offset for lock */
        //     off_t l_len;     /* Number of bytes to lock */
        //     pid_t l_pid;     /* PID of process blocking our lock
        //                         (set by F_GETLK and F_OFD_GETLK) */
        //     ...
        // };
        printf("  l_type: %d\n", data->l_type);
        printf("  l_whence: %d\n", data->l_whence);
        printf("  l_start: %ld\n", data->l_start);
        printf("  l_len: %ld\n", data->l_len);
        printf("  l_pid: %d\n", data->l_pid);
    } else if (value.type == AT_STRUCT_F_OWNER_EX) {
        // struct f_owner_ex {
        //     int   type;
        //     pid_t pid;
        // };
        struct f_owner_ex*  data = (struct f_owner_ex*) value.str.c_str();
        printf("  type: %d\n", data->type);
        printf("  pid: %d\n", data->pid);
    }
}

int StructFlockPrinter::getStructLength(argument_type type) {
    if (type == AT_STRUCT_FLOCK) {
        return sizeof(flock);
    } else if (type == AT_STRUCT_F_OWNER_EX) {
        return sizeof(f_owner_ex);
    } else {
        return -1;
    }
}