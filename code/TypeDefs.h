# ifndef MSC_TYPEDEFS_H
# define MSC_TYPEDEFS_H

#include "libs/magic_enum.hpp" // to print out enum names
#include <string>

enum argument_type {
    AT_NONE,
    AT_VOID_ADDR,
    AT_INT,
    AT_LONG,
    AT_CHAR_PTR, // 4
    AT_SIZE_T,
    AT_OUT_BUFFER, //6
    AT_OFF_T,
    AT_STRUCT_STAT, //8
    AT_MODE_T,
    AT_UID_T,
    AT_STRUCT_SOCKADDR, //11
    AT_STRUCT_CAP_USER_HEADER_T, //12
    AT_STRUCT_CAP_USER_DATA_T, //13
    AT_STRUCT_POLLFD,
    AT_STRUCT_SIGACTION,
    AT_STRUCT_SET_ITIMERVAL, //16
    AT_SIGSET_T,
    AT_SIGSET_T_PTR,
    AT_SOCKLEN_T, //18
    AT_VOID_PTR_FOLLOW, //19
    AT_STRUCT_MSGHDR,
    AT_STRUCT_F_OWNER_EX,
    AT_STRUCT_FLOCK,
    AT_UINT64_PTR, // pointer to an int (int*), sperrips reads, saves and restores the value behind the address
    AT_IGNORE, // means that this argument must be ignored
    AT_UINT64_PTR_PTR,
    AT_STRUCT_UTSNAME,
    AT_STRUCT_TIMESPEC,
    AT_STRUCT_SYSINFO,
    AT_SIGNED_INT,
    AT_STRUCT_STATFS,
    AT_STRUCT_RLIMIT64,
    AT_PID_T,
    AT_STRUCT_ROBUST_LIST_HEAD,
    AT_STRUCT_RUSAGE,
    AT_SOCKLEN_T_PTR,
    AT_STRUCT_TIMEVAL,
    AT_STRUCT_IOVEC,
    AT_PASLR_FOLLOW,
    AT_STRUCT_TIMEZONE
};

enum VERBOSITY {
    LOW = 0,
    NORMAL = 1,
    HIGH = 2,
    VERYHIGH = 3
};

struct argument_value {
    argument_type type;
    uint64_t int64;
    std::string str = std::string();
    void* ptr = nullptr;
    void* related = nullptr;
};

enum REGISTER_ORDER {
    RDI,
    RSI,
    RDX,
    RCX,
    R8,
    R9
};

// Do not forget to add string representation in Utils::getExitCodeName
enum exit_code {
    CORRECT = 0,
    FAILED_PARSING_RECFILE = 1 << 0,
    COMPARING_DIFFERENT_JOBS = 1 << 1,
    POTENTIAL_MEMORY_CORRUPTION = 1 << 2,
    DIFFERENT_SYSCALLS_ENCOUNTERED = 1 << 3,
    WRONG_POINTER = 1 << 4,
    UNEXPECTED_MEMORY_CONTENT = 1 << 5,
    UNEXPECTED_ARGUMENT = 1 << 6,
    UNEXPECTED_ARGUMENT_VALUE = 1 << 7,
    UNKNOWN_PARAMETER_TYPE = 1 << 8,
    ERROR_WRITING_TO_CHILD = 1 << 9,
    WRONG_INTEGER =  1 << 10,
    STAT_DIFFERS = 1 << 11,
    UNIMPLEMENTED_SYSCALL = 1 << 12,
    UNDEFINED_SYSTEM_STATE = 1 << 13,
    UNAME_DIFFERENT = 1 << 14,
    LAST  = 1 << 14 // used for loop must be last
};

inline exit_code operator | (exit_code lhs, exit_code rhs) {
    using T = std::underlying_type_t <exit_code>;
    return static_cast<exit_code>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

#endif