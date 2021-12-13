#include "Utils.h"

/**
 * @brief Dumps data from a memory location to stdout as hexvalues.
 *
 * @param mem Memory location to read from.
 * @param n Amount of bytes to read.
 * @param indent prefix padding for output.
 */

void Utils::dump(const void* mem, unsigned int n, std::string indent) {
    // Adapted form https://stackoverflow.com/a/1286761/2835625
    const unsigned char* p = reinterpret_cast< const unsigned char*>(mem);
    std::cout << indent << "0x";

    for (unsigned int i = 0; i < n; i++) {
        printf("%02X ", int(p[i]));
    }

    std::cout << std::endl;
}

/**
 * @brief Indicates whether the given type is an integer.
 *
 * @param type Type to check
 * @return true given type is an integer.
 * @return false given type is not an integer.
 */

bool Utils::isInt(argument_type type) {
    std::set<argument_type> ints {AT_INT, AT_SIGNED_INT, AT_LONG, AT_SIZE_T, AT_OFF_T, AT_MODE_T, AT_UID_T, AT_SIGSET_T, AT_SOCKLEN_T, AT_PID_T};
    return ints.find(type) != ints.end();
}

/**
 * @brief Indicates whether the given type is a struct.
 *
 * @param type Type to check
 * @return true given type is a struct.
 * @return false given type is no struct.
 */

bool Utils::isStruct(argument_type type) {
    std::set<argument_type> structs {AT_STRUCT_STAT, AT_STRUCT_SOCKADDR, AT_STRUCT_CAP_USER_HEADER_T, AT_STRUCT_CAP_USER_DATA_T, AT_STRUCT_POLLFD, AT_STRUCT_SIGACTION, AT_STRUCT_SET_ITIMERVAL, AT_STRUCT_MSGHDR, AT_STRUCT_F_OWNER_EX, AT_STRUCT_FLOCK, AT_STRUCT_UTSNAME, AT_STRUCT_TIMESPEC, AT_STRUCT_SYSINFO, AT_STRUCT_STATFS, AT_STRUCT_RLIMIT64, AT_STRUCT_ROBUST_LIST_HEAD, AT_STRUCT_RUSAGE, AT_STRUCT_TIMEVAL, AT_STRUCT_IOVEC, AT_STRUCT_TIMEZONE};
    return structs.find(type) != structs.end();
}

/**
 * @brief Indicates whether the given type should be onexit of a syscall.
 *
 * @param type Type to check
 * @return true given type should be obtained on syscall exit.
 * @return false given type should be obtained on syscall entry.
 */

bool Utils::isAfterCall(argument_type type) {
    std::set<argument_type> after {AT_OUT_BUFFER, AT_SOCKLEN_T_PTR, AT_VOID_PTR_FOLLOW, AT_SIGSET_T_PTR, AT_PASLR_FOLLOW};
    return (after.find(type) != after.end()) || isStruct(type);
}

/**
 * @brief Indicates whether the given type is a pointer.
 *
 * @param type Type to check
 * @return true given type is a pointer.
 * @return false given type is no pointer.
 */

bool Utils::isPointer(argument_type type) {
    std::set<argument_type> after {AT_SOCKLEN_T_PTR, AT_VOID_PTR_FOLLOW, AT_SIGSET_T_PTR, AT_UINT64_PTR, AT_UINT64_PTR_PTR, AT_SOCKLEN_T_PTR, AT_PASLR_FOLLOW};
    return (after.find(type) != after.end()) || isStruct(type);
}

/**
 * @brief Parses the output of /proc/PID/maps into the respective protbuf object.
 *
 * @param mapping_file Content of /proc/PID/maps.
 * @return msc::ASLRInfo* Target protobuf object.
 */

msc::ASLRInfo* Utils::parseASLRMapping(std::string mapping_file) {
    msc::ASLRInfo* aslrInfo = new msc::ASLRInfo();
    std::ifstream randomize_va_space("/proc/sys/kernel/randomize_va_space");
    std::string line;

    if (randomize_va_space.is_open()) {
        while (getline(randomize_va_space, line)) {
            aslrInfo->set_state(std::stoi(line));
            break;
        }

        randomize_va_space.close();
    }

    std::istringstream str(mapping_file);

    while (std::getline(str, line)) {
        msc::MemoryRange* range = aslrInfo->add_ranges();
        uintptr_t from = (uintptr_t) strtoul(line.c_str(), NULL, 16);
        const char* ptr = (strstr(line.c_str(), "-") + 1); // plus one to skip first char, which is "-"
        uintptr_t to = (uintptr_t) strtoul(ptr, NULL, 16);
        std::string section = "";

        if (ptr = strstr(line.c_str(), "/")) {
            section = std::string(ptr);
        } else if (ptr = strstr(line.c_str(), "[")) {
            section = std::string(ptr);
        }

        range->set_from(from);
        range->set_to(to);
        range->set_section(section);
    }

    return aslrInfo;
}

/**
 * @brief Returns the string representation of a given exit_code.
 * The values are too large to resolve them via magic_enum::enum_name.
 *
 * @param code exit_code to get the name
 * @return std::string String representation.
 */

// Extract form TypeDefs.h via echo """copied defs from TypeDefs.h""" | cut -d "=" -f1 | tr -d " " | xargs -i echo "case {}: return \"{}\";"
std::string Utils::getExitCodeName(exit_code code) {
    switch (code) {
    case CORRECT:
        return "CORREECT";

    case FAILED_PARSING_RECFILE :
        return "FAILED_PARSING_RECFILE";

    case COMPARING_DIFFERENT_JOBS :
        return "COMPARING_DIFFERENT_JOBS";

    case POTENTIAL_MEMORY_CORRUPTION :
        return "POTENTIAL_MEMORY_CORRUPTION";

    case DIFFERENT_SYSCALLS_ENCOUNTERED :
        return "DIFFERENT_SYSCALLS_ENCOUNTERED";

    case WRONG_POINTER :
        return "WRONG_POINTER";

    case UNEXPECTED_MEMORY_CONTENT :
        return "UNEXPECTED_MEMORY_CONTENT";

    case UNEXPECTED_ARGUMENT :
        return "UNEXPECTED_ARGUMENT";

    case UNEXPECTED_ARGUMENT_VALUE :
        return "UNEXPECTED_ARGUMENT_VALUE";

    case UNKNOWN_PARAMETER_TYPE :
        return "UNKNOWN_PARAMETER_TYPE";

    case ERROR_WRITING_TO_CHILD :
        return "ERROR_WRITING_TO_CHILD";

    case WRONG_INTEGER :
        return "WRONG_INTEGER";

    case STAT_DIFFERS :
        return "STAT_DIFFERS";

    case UNIMPLEMENTED_SYSCALL :
        return "UNIMPLEMENTED_SYSCALL";

    case UNDEFINED_SYSTEM_STATE :
        return "UNDEFINED_SYSTEM_STATE";

    case UNAME_DIFFERENT :
        return "UNAME_DIFFERENT";

    default:
        return "UNKNONW_EXIT_CODE";
    }
}