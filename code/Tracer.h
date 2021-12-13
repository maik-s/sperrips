# ifndef MSC_TRACER_H
# define MSC_TRACER_H

#include <sys/ptrace.h>
#include <vector>
#include <string>
#include <string.h>
#include <sys/user.h>
#include <iostream>
#include <cstdlib>
#include <sys/syscall.h>
#include <sys/personality.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "handler/Handler.h"
#include <variant>
#include <memory>
#include "Recorder.h"

class Tracer {
private:
    int verbosityLevel;
    bool recordingMode;
    int syscall_counter = 1;
    void retrieveArgumentValue(argument_type type, __ptrace_syscall_info callinfo, int index, argument_value& argVal);
    void retrieveReturnValue(__ptrace_syscall_info callinfo);
    void saveArguments(std::unique_ptr<Handler>& handler, __ptrace_syscall_info callinfo, bool onEntry);
    void read_struct(void* val, size_t length, argument_value& argVal);
    msc::ASLRInfo* aslrInfo = nullptr;
public:
    exit_code ec = CORRECT;
    Tracer(std::vector<std::string> args, std::string stdin_msg, bool recordingMode, int pVerbosityLevel, std::string jobname, std::string stdout_file, std::string stderr_file,  std::string pathPrefix);
    int wait();
    void ptracef(enum __ptrace_request req, void* addr, void* data);
    void ptracef(enum __ptrace_request req, pid_t pid, void* addr, void* data);
    std::unique_ptr<Handler> getHandler(__ptrace_syscall_info callinfo);
    std::string read_string(unsigned long addr);
    void handleSyscall();
    std::string readnbytes(void* ptr, size_t length);
    void finish(exit_code exitcode);
    bool isRecordingMode();
    exit_code write_to_child(unsigned long addr, std::string data, size_t oldSize);
    bool checkState(bool policy_lax);
    msc::ASLRInfo* retrieveASLRInfo();
};

#endif