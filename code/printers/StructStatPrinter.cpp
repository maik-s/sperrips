#include "StructStatPrinter.h"

void StructStatPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct stat* data = (struct stat*) value.str.c_str();

    printf("  st_dev: %ld\n", data->st_dev);

    printf("  st_ino: %ld\n", data->st_ino);

    printf("  st_mode: %d\n", data->st_mode);

    printf("  st_nlink: %ld\n", data->st_nlink);

    printf("  st_uid: %d\n", data->st_uid);

    printf("  st_gid: %d\n", data->st_gid);

    printf("  st_rdev: %ld\n", data->st_rdev);

    printf("  st_size: %ld\n", data->st_size);

    printf("  st_blksize: %ld\n", data->st_blksize);

    printf("  st_blocks: %ld\n", data->st_blocks);

    printf("  st_atime: %ld\n", data->st_atime);

    printf("  st_mtime: %ld\n", data->st_mtime);

    printf("  st_ctime: %ld\n", data->st_ctime);
}

int StructStatPrinter::getStructLength(argument_type type) {
    return sizeof(struct stat);
}