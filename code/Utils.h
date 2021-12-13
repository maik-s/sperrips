# ifndef MSC_UTILS_H
# define MSC_UTILS_H
#include <iostream>
#include <set>
#include <vector>
#include "TypeDefs.h"
#include <memory> // unique_ptr

#include <sys/socket.h> // struct sockaddr, msghdr
#include <sys/time.h> // struct timespec, setitimerval
#include <cstring> // strstr
#include <sstream> // getline
#include <iostream> //std::istringstream 
#include "proto_files/cpp_out/Syscall.pb.h" // msc::ASLRmeta
#include <fstream>

class Utils {
public:
    static void dump(const void* mem, unsigned int n, std::string indent);
    static bool isInt(argument_type type);
    static bool isStruct(argument_type type);
    static bool isAfterCall(argument_type type);
    static bool isPointer(argument_type type);
    static msc::ASLRInfo* parseASLRMapping(std::string mapping_file);
    static std::string getExitCodeName(exit_code code);
};
#endif