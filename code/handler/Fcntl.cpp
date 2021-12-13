#include "Fcntl.h"
#include <fcntl.h>

std::vector<argument_type> Fcntl::getArgumentTypes() {
    if (_argumentTypes.empty()) {
        _argumentTypes = std::vector<argument_type>({AT_INT, AT_INT, AT_LONG});
    }

    return _argumentTypes;
}


void Fcntl::rewriteArgumentType(int argumentIndex, argument_value argumentValue) {
    if (argumentIndex != 1) {
        return;
    }

    if (argumentValue.int64 == F_DUPFD || argumentValue.int64 == F_NOTIFY || argumentValue.int64 == F_SETFD || argumentValue.int64 == F_SETFL || argumentValue.int64 == F_SETLEASE || argumentValue.int64 == F_SETOWN || argumentValue.int64 == F_SETSIG || argumentValue.int64 == F_DUPFD_CLOEXEC || argumentValue.int64 == F_SETPIPE_SZ || argumentValue.int64 == F_ADD_SEALS) {
        _argumentTypes.at(2) = AT_INT;
        // AT_INT
    } else if (argumentValue.int64 == F_GETOWN_EX || argumentValue.int64 == F_SETOWN_EX) {
        _argumentTypes.at(2) = AT_STRUCT_F_OWNER_EX;
        //  (struct f_owner_ex *)
    } else if (argumentValue.int64 == F_GETLK || argumentValue.int64 == F_OFD_GETLK || argumentValue.int64 == F_OFD_SETLK || argumentValue.int64 == F_OFD_SETLKW || argumentValue.int64 == F_SETLK || argumentValue.int64 == F_SETLKW) {
        //  (struct flock *)
        _argumentTypes.at(2) = AT_STRUCT_FLOCK;
    } else if (argumentValue.int64 == F_GET_FILE_RW_HINT || argumentValue.int64 == F_GET_RW_HINT || argumentValue.int64 == F_SET_FILE_RW_HINT || argumentValue.int64 == F_SET_RW_HINT) {
        _argumentTypes.at(2) = AT_UINT64_PTR;
        //  (uint64_t *; since Linux 4.13)
    } else if (argumentValue.int64 == F_GETFD || argumentValue.int64 == F_GETFL || argumentValue.int64 == F_GETLEASE || argumentValue.int64 == F_GETOWN || argumentValue.int64 == F_GETSIG || argumentValue.int64 == F_GETPIPE_SZ || argumentValue.int64 == F_GET_SEALS) {
        //  (void)
        _argumentTypes.at(2) = AT_IGNORE;
    }

    printf("[i] Argument %d rewritten to type %s\n", 2, ((std::string)magic_enum::enum_name(_argumentTypes.at(2))).c_str());
}