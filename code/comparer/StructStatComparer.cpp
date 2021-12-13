#include "ArgumentComparer.h"
#include "StructStatComparer.h"

bool StructStatComparer::compare(struct stat* recorded, struct stat* replayed) {
    bool same = true;

    if (recorded->st_dev != replayed->st_dev) {
        logMessage("st_dev not equal (" + std::to_string(recorded->st_dev) + " <-> " + std::to_string(replayed->st_dev) + ") (accaptable)");
        // same = false;
    }

    if (recorded->st_ino != replayed->st_ino) {
        logMessage("st_ino not equal (" + std::to_string(recorded->st_ino)  + " <-> " + std::to_string(replayed->st_ino) + ") (accaptable)");
        // same = false;
    }

    if (recorded->st_mode != replayed->st_mode) {
        logMessage("st_mode not equal");
        same = false;
    }

    if (recorded->st_nlink != replayed->st_nlink) {
        logMessage("st_nlink not equal");
        same = false;
    }

    if (recorded->st_uid != replayed->st_uid) {
        logMessage("st_uid not equal");
        same = false;
    }

    if (recorded->st_gid != replayed->st_gid) {
        logMessage("st_gid not equal (" + std::to_string(recorded->st_gid)  + " <-> " + std::to_string(replayed->st_gid) + ")");
        same = false;
    }

    if (recorded->st_rdev != replayed->st_rdev) {
        logMessage("st_rdev not equal");
        same = false;
    }

    if (recorded->st_size != replayed->st_size) {
        logMessage("st_size not equal");
        same = false;
    }

    if (recorded->st_blksize != replayed->st_blksize) {
        logMessage("st_blksize not equal");
        same = false;
    }

    if (recorded->st_blocks != replayed->st_blocks) {
        logMessage("st_blocks not equal");
        same = false;
    }

    if (recorded->st_atime != replayed->st_atime) {
        logMessage("st_atime not equal (accaptable)");
        // same = false;
    }

    if (recorded->st_mtime != replayed->st_mtime) {
        logMessage("st_mtime not equal (accaptable)");
        // same = false;
    }

    if (recorded->st_ctime != replayed->st_ctime) {
        logMessage("st_ctime not equal (accaptable)");
        // same = false;
    }

    return same;
}

void StructStatComparer::logMessage(std::string message) {
    std::cout << "[StructStatComparer] " << message.c_str() << std::endl;
}