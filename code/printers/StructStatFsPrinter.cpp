#include "StructStatFsPrinter.h"

void StructStatFsPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct statfs* data = (struct statfs*) value.str.c_str();

    // struct statfs {
    //            __fsword_t f_type;    /* Type of filesystem (see below) */
    //            __fsword_t f_bsize;   /* Optimal transfer block size */
    //            fsblkcnt_t f_blocks;  /* Total data blocks in filesystem */
    //            fsblkcnt_t f_bfree;   /* Free blocks in filesystem */
    //            fsblkcnt_t f_bavail;  /* Free blocks available to
    //                                     unprivileged user */
    //            fsfilcnt_t f_files;   /* Total inodes in filesystem */
    //            fsfilcnt_t f_ffree;   /* Free inodes in filesystem */
    //            fsid_t     f_fsid;    /* Filesystem ID */
    //            __fsword_t f_namelen; /* Maximum length of filenames */
    //            __fsword_t f_frsize;  /* Fragment size (since Linux 2.6) */
    //            __fsword_t f_flags;   /* Mount flags of filesystem
    //                                     (since Linux 2.6.36) */
    //            __fsword_t f_spare[xxx];
    //                            /* Padding bytes reserved for future use */
    //        };
    printf("  f_type: %ld\n", data->f_type);

    printf("  f_bsize: %ld\n", data->f_bsize);

    printf("  f_blocks:%ld\n", data->f_blocks);

    printf("  f_bfree: %ld\n", data->f_bfree);

    printf("  f_bavail: %ld\n", data->f_bavail);

    printf("  f_files: %ld\n", data->f_files);

    printf("  f_ffree: %ld\n", data->f_ffree);

    printf("  f_fsid: %p\n", &data->f_fsid);

    printf("  f_namelen %ld\n", data->f_namelen);

    printf("  f_frsize: %ld\n", data->f_frsize);

    printf("  f_flags: %ld\n", data->f_flags);

    printf("  f_spare: %ln\n", data->f_spare);

}

int StructStatFsPrinter::getStructLength(argument_type type) {
    return sizeof(struct statfs);
}