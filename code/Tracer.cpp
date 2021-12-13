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
#include <sys/uio.h>
#include "Tracer.h"
#include "TypeDefs.h"
#include "Utils.h"
#include <vector>
#include "RecordReplay.h"
#include "Replayer.h"

#include "handler/Handler.h"
#include "handler/HandlerRegistry.h"
#include <sys/socket.h> // struct sockaddr, msghdr, socklen_t
#include <cmath> // pow

REGISTER_HANDLER(Handler, -1, Handler::create);

#define PIPE_READ 0
#define PIPE_WRITE 1

#define ON_ENTRY true
#define ON_EXIT false

/**
 * @brief Wrapper function for lib's `ptrace` function.
 * It has some additional handling included such as checking for occurred errors.
 *
 * @param req ptrace request argument.
 * @param pid ptrace pid argument.
 * @param addr ptrace addr argument
 * @param data ptrace data argument.
 */
void Tracer::ptracef(enum __ptrace_request req, pid_t pid, void* addr, void* data) {
    //std::cout << "ptracf: " << req << ", pid: " << pid << std::endl;
    auto out = ptrace(req, pid, addr, data);

    if (out != 0) {
        if (req == PTRACE_KILL) {
            std::cout << "Killing child" << std::endl;
        } else if (req != PTRACE_GET_SYSCALL_INFO) {
            std::cerr << "err: " << out << "\n";
        }
    }

    if (out == -1) {
        std::cerr << "an error occurred (" << out << ") !\n";
        perror("ptrace");
    }
}
pid_t child;
std::unique_ptr<Handler> handler;
std::unique_ptr<Recorder> recorder;
std::unique_ptr<Replayer> replayer;
std::unique_ptr<RecordReplay> recordreplay;
struct __ptrace_syscall_info savedCallinfo = {};
bool nohandler = false;

/**
 * @brief Construct a new Tracer:: Tracer object It is the central component for tracing a tracee.
 *
 * @param args The command-line arguments of the tracee.
 * @param stdinmsg Stdin input for the tracee.
 * @param pRecordingMode Indicates whether sperrips operates in recording or replaying mode.
 * @param pVerbosityLevel Sets the verbosity level of ouput messages.
 * @param jobName The configured jobname.
 * @param stdout_filepath Path to stores the tracee's stdout messages. Can be same as \p stderr_filepath.
 * @param stderr_filepath Path to store the tracee's stderr messages. Can be same as \p stdout_filepath.
 * @param pathPrefix Hostname abosolute path prefix for the recording and replaying files.
 */
Tracer::Tracer(std::vector<std::string> args, std::string stdinmsg, bool pRecordingMode, int pVerbosityLevel, std::string jobName, std::string stdout_filepath, std::string stderr_filepath, std::string pathPrefix) {
    verbosityLevel = pVerbosityLevel;
    recordingMode = pRecordingMode;

    std::vector<char*> largv;

    for (std::string const& str : args) {
        largv.push_back(const_cast<char*>(str.c_str()));
    }

    largv.push_back(NULL);

    int aStdinPipe[2];
    int aStdoutPipe[2];
    int aStderrPipe[2];
    int nResult;

    // https://stackoverflow.com/a/12839498/2835625
    if (pipe(aStdinPipe) < 0) {
        perror("allocating pipe for child input redirect");
        exit(errno);
    }

    if (pipe(aStdoutPipe) < 0) {
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        perror("allocating pipe for child output redirect");
        exit(errno);
    }

    if (pipe(aStderrPipe) < 0) {
        close(aStderrPipe[PIPE_READ]);
        close(aStderrPipe[PIPE_WRITE]);
        perror("allocating pipe for child output redirect");
        exit(errno);
    }

    child = fork();

    // Do not add logic before checking if we are in child or not
    // Otherwise, everything after the above fork() is executed twice till here
    if (child == 0) {
        freopen(stdout_filepath.c_str(), "w", stdout);  // empty the file
        freopen(stderr_filepath.c_str(), "w", stderr);  // empty the file

        if (dup2(aStdinPipe[PIPE_READ], STDIN_FILENO) == -1) {
            exit(errno);
        }

        if (dup2(aStdoutPipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
            exit(errno);
        }

        if (dup2(aStderrPipe[PIPE_WRITE], STDERR_FILENO) == -1) {
            exit(errno);
        }

        // all these are for use by parent only
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);

        freopen(stdout_filepath.c_str(), "a+", stdout);
        freopen(stderr_filepath.c_str(), "a+", stderr);
        ptracef(PTRACE_TRACEME, 0, NULL, NULL);
        nResult = execve(args.at(0).c_str(), largv.data(), NULL);
        return;
    } else if (child > 0) {
        // parent continues here
        // close unused file descriptors, these are for child only
        close(aStdinPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);
        close(aStderrPipe[PIPE_WRITE]);

        // Include error check here
        if (stdinmsg.size() > 0) {
            write(aStdinPipe[PIPE_WRITE], stdinmsg.c_str(), stdinmsg.size());
        }

        // done with these in this example program, you would normally keep these
        // open of course as long as you want to talk to the child
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStderrPipe[PIPE_READ]);
    } else {
        // failed to create child
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStderrPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);
        close(aStderrPipe[PIPE_WRITE]);
        exit(-1);
    }

    if (this->isRecordingMode()) {
        recorder = std::make_unique<Recorder>(jobName, pathPrefix);
        recorder->addCommand(args, stdinmsg);
    } else {
        replayer = std::make_unique<Replayer>(jobName, pathPrefix);
    }

    this->aslrInfo = this->retrieveASLRInfo(); // init aslrInfo. we need it for replaying
    std::cout << std::endl;
}

/**
 * @brief Returns the waitpid status of the tracee.
 *
 * @return int waitpid status of the tracee.
 */
int Tracer::wait() {
    int wait_status = 0;
    waitpid(child, &wait_status, 0);
    //std::cout << "Stp: " << WSTOPSIG(wait_status) << ";" << SIGTRAP << "\n";
    return wait_status;
}


/**
 * @brief Wrapper function for ptracef to automatically add current child's pid.
 *
 * @param req ptracef request.
 * @param addr ptracef addr
 * @param data ptracef data
 */
void Tracer::ptracef(enum __ptrace_request req, void* addr, void* data) {
    ptracef(req, child, addr, data);
}

/**
 * @brief Reads a string from the tracee's virtual address space.
 * A string's end is determined by the first appearing null-byte.
 *
 * @param addr Memory address of the tracee's virtual address space.
 * @return std::string Read string.
 */
std::string Tracer::read_string(unsigned long addr) {
    // Adapted from: https://stackoverflow.com/a/10515249/2835625
    int allocated = 128, read = 0;
    char* val = (char*)malloc(allocated);

    if (!val)
        return NULL;

    ssize_t nread;
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_len = allocated;
    unsigned long tmp = 0;

    while (1) {
        local[0].iov_base = val + read;

        if (read + 8 > allocated) {
            allocated *= 2;
            char* temp_val = (char*)realloc((void*) val, allocated);

            if (!temp_val) {
                free(val);
                return NULL;
            }

            val = temp_val;
            local[0].iov_base = val + read;
        }

        local[0].iov_len = allocated - read;
        remote[0].iov_base = (char*) addr + read;
        remote[0].iov_len = 8;
        errno = 0;
        nread = process_vm_readv(child, local, 1, remote, 1, 0);

        if (errno == -1 || strnlen((char*)local[0].iov_base, 8) < 8) {
            if (errno) {
                std::cerr << "read_string errno: " << errno << std::endl;
            }

            ((char*)local[0].iov_base)[nread] = 0;

            break;
        }

        read += nread;
    }

    std::string res(val);
    free(val);
    return res;
}

/**
 * @brief Write data into the child's (tracee's) virtual memory.
 * May add `POTENTIAL_MEMORY_CORRUPTION` or `ERROR_WRITING_TO_CHILD` to the global exit_code variable and respectively return the exit_code.
 * `POTENTIAL_MEMORY_CORRUPTION` appears if \p oldSize is less than the length of \p data.
 * `ERROR_WRITING_TO_CHILD` is indicated by errno of ptrace
 *
 * @param addr Memory location to write to.
 * @param data Data to write.
 * @param oldSize Length of the appropriate data obtained in the recording phase.
 * @return exit_code Exit code of the operating.
 */
exit_code Tracer::write_to_child(unsigned long addr, std::string data, size_t oldSize) {
    void* child_addr = reinterpret_cast<void*>(addr);
    const int sizeoflong = sizeof(long);
    union helperunion { // we use this union to access the characters as long values, needed for PTRACE_POKETEXT
        long val;
        char chars[sizeoflong];
    } mem_data;
    std::vector<long> data_words;

    if (oldSize >= data.length()) {
        // no problematic case: null remaining memory
    } else {
        std::cerr << "[Critical]! Potential memory corruption in child as " << (int)(data.length() - oldSize) << " bytes overflow @" << child_addr << std::endl;
        ec = ec | POTENTIAL_MEMORY_CORRUPTION;
        return (POTENTIAL_MEMORY_CORRUPTION);
    }

    for (int i = 0; i < data.length() / sizeoflong; i++) {
        std::string substr = data.substr(i * sizeoflong, sizeoflong);
        mem_data.val = 0; // clear memory
        memcpy(mem_data.chars, substr.c_str(), sizeoflong);
        data_words.push_back(mem_data.val);
    }

    int remaining = data.length() % sizeoflong;

    if (remaining != 0) {
        std::string substr = data.substr(data.length() - remaining, sizeoflong);
        mem_data.val = 0; // clear memory
        memcpy(mem_data.chars, substr.c_str(), substr.length());
        data_words.push_back(mem_data.val);
    }

    if (oldSize > data.length()) {
        for (int i = 0; i < (oldSize - data.length()) / sizeoflong; i++) {
            mem_data.val = 0; // clear memory
            data_words.push_back(mem_data.val);
        }
    }

    for (int i = 0; i < data_words.size(); i++) {
        errno = 0;
        void* rvalue = reinterpret_cast<void*>(data_words.at(i));
        void* child_addr_new = reinterpret_cast<void*>(addr + (i * sizeoflong));
        int res = ptrace(PTRACE_POKEDATA, child, child_addr_new, rvalue);

        if (errno) {
            perror("write_to_child");
            ec = ec | ERROR_WRITING_TO_CHILD;
            return ERROR_WRITING_TO_CHILD;
        }
    }

    return CORRECT;
}

/**
 * @brief Reads \p length bytes of the tracee's virtual address space at location \p ptr.
 *
 * @param ptr Location to read from.
 * @param length Number of bytes to read.
 * @return std::string Read bytes.
 */

std::string Tracer::readnbytes(void* ptr, size_t length) {
    char* val = (char*)malloc(length);

    if (!val || ptr == nullptr || length == 0)
        return std::string();

    const int sizeoflong = sizeof(long);
    union helperunion { // we use this union to access the characters as long values, needed for PTRACE_PEEKTEXT
        long val;
        char chars[sizeoflong];
    } mem_data;

    char* child_addr = reinterpret_cast<char*>(ptr);
    std::string result = std::string();
    errno = 0;

    for (int i = 0; i < length / sizeoflong; i++) {
        mem_data.val = 0; // clear memory
        char* addr =  child_addr + (i * 8);
        mem_data.val = ptrace(PTRACE_PEEKDATA, child, addr, NULL);

        if (errno) {
            perror("readnbytes1");
            return std::string();
        }

        result += std::string(mem_data.chars, 8);
    }

    int remaining = length % sizeoflong;

    if (remaining != 0) {
        char* addr = child_addr + ((length / sizeoflong) * 8);

        if (length < sizeoflong) {
            addr = child_addr;
        }

        mem_data.val = 0; // clear memory
        mem_data.val = ptrace(PTRACE_PEEKDATA, child, addr, NULL);

        if (errno) perror("readnbytes2");

        std::string str = std::string(mem_data.chars, sizeoflong);
        std::string sub = str.substr(0, remaining);
        result += sub;
    }

    if (result.size() != length) {
        std::cerr << "[Error] wrong content! expected length " << length << ", actual length: " << result.size() << std::endl;
    }

    return result;
}

/**
 * @brief Parses `cat /proc/childpid/maps` and returns the mapping.
 *
 * @return msc::ASLRInfo* Obtained memory mapping.
 */
msc::ASLRInfo* Tracer::retrieveASLRInfo() {
    // https://stackoverflow.com/a/478960/2835625
    char cmd[0x40] = {0};
    sprintf(cmd, "cat /proc/%d/maps", child);
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return Utils::parseASLRMapping(result);
}

/**
 * @brief  Parses the data in \p pCallinfo and stores them accordingly as an argument_type.
 * All argument_types are saved as a vector in handler.
 *
 * @param handler The current syscall handler.
 * @param pCallinfo Current struct as retrieved via ptrace.
 * @param onEntry Indicate whether arguments are obtained onEntry or onExit of a syscal..
 */
void Tracer::saveArguments(std::unique_ptr<Handler>& handler, __ptrace_syscall_info pCallinfo, bool onEntry) {
    std::vector<argument_type> args = handler->getArgumentTypes();
    std::vector<argument_value> argVals(0);

    for (int i = 0; i < args.size(); i++) {
        argument_type type = args.at(i);
        argument_value val = argument_value();

        retrieveArgumentValue(type, pCallinfo, i, val);

        if (handler->hasDependingArguments()) {
            handler->rewriteArgumentType(i, val);
        }

        argVals.push_back(val);
    }

    args = handler->getArgumentTypes();

    for (int i = 0; i < argVals.size() && handler->hasDependingArguments(); i++) {
        // update all argument types afterwards.
        // Even though it's n^2 currently there is no better implementation. We dont know which argument index has changed to what value. Hence, we walk across everything
        argVals.at(i).type = args.at(i);
    }

    handler->setArgumentValues(argVals);
}

/**
 * @brief Handles a syscall interception.
 * First, it checks whether a syscalls has been intercepted onEntry or onExit.
 * Depending on sperrips' phase (recording or replaying) it either records a value,
 * or checks its equality and may replays it.
 * Potential error codes are added to the global error code state.
 */

void Tracer::handleSyscall() {
    struct __ptrace_syscall_info callinfo = {};
    struct user_regs_struct regs = {};
    ptracef(PTRACE_GET_SYSCALL_INFO, (void*)sizeof(callinfo), &callinfo);
    ptrace(PTRACE_GETREGS, child, 0, &regs);

    if (callinfo.op == PTRACE_SYSCALL_INFO_ENTRY) {
        // we always need to save the callinfo, for rereading, after replaying
        memcpy((void*)&savedCallinfo, (void*)&callinfo, sizeof(struct __ptrace_syscall_info));
        handler = getHandler(callinfo);
        saveArguments(handler, callinfo, ON_ENTRY);

        if (this->isRecordingMode()) {
            if (handler->isExitCall()) {
                // if it is an exit call (e.g. exit or group_exit) we need to handle it here, because we wont get into the PTRACE_SYSCALL_INFO_EXIT block.
                handler->setReturnValue({}); // Set return value, to avoid later memory leaks, but in fact it is not known, as we cannot trace it after child's death
                handler->print();
                this->aslrInfo = this->retrieveASLRInfo();
            }

            recorder->addSyscall(handler.get(), ON_ENTRY);
        } else {
            if (replayer->isCorrectType(callinfo.entry.nr)) {
                if (nohandler == false || replayer->getCurrentSyscall().type() != -1) {
                    exit_code resultCompare = replayer->compareArgumentsAndReturnValue(handler, false, ON_ENTRY);
                    exit_code resReplay;
                    replayer->addArguments(handler, ON_ENTRY);
                    bool wasSame = resultCompare == CORRECT;

                    if (!wasSame) {
                        ec = ec | resultCompare;
                        resReplay = replayer->replayArgumentsAndReturnValue(handler, &callinfo, &regs, this, ON_ENTRY);
                        ptrace(PTRACE_SETREGS, child, 0, &regs);
                        saveArguments(handler, callinfo, ON_ENTRY);
                    }

                    replayer->addDifferencePair(!wasSame, ON_ENTRY);
                } else {
                    std::cerr << "Unimplemented syscall" << std::endl;
                    ec = ec | UNIMPLEMENTED_SYSCALL;
                }
            } else {
                auto expected = magic_enum::enum_name(replayer->getCurrentSyscall().type());
                auto actual = magic_enum::enum_name((msc::SyscallOpcode) callinfo.entry.nr);
                fflush(stdout);
                fflush(stderr);
                std::cerr << "ERROR: The programm executed different syscalls. (Expected: " << expected << ", actual: " << actual <<  "). This is unacceptable. Exiting" << std::endl;
                ec = ec | (DIFFERENT_SYSCALLS_ENCOUNTERED);
            }
        }
    } else if (callinfo.op == PTRACE_SYSCALL_INFO_EXIT) {
        std::cout << "Syscall done" << std::endl;
        syscall_counter += 1;

        this->retrieveReturnValue(callinfo); // also sets the return value in the handler
        saveArguments(handler, savedCallinfo, ON_EXIT);

        if (this->isRecordingMode()) {
            recorder->addSyscall(handler.get(), ON_EXIT);
        } else {
            if (nohandler == false || replayer->getCurrentSyscall().type() != -1) {
                exit_code resultCompare = replayer->compareArgumentsAndReturnValue(handler, true, ON_EXIT);
                exit_code resReplay;
                replayer->setReturnValue(handler);
                replayer->addArguments(handler, ON_EXIT); // save arguments, before they are replayed
                bool wasSame = resultCompare == CORRECT;

                if (!wasSame) {
                    ec = ec | resultCompare;
                    resReplay = replayer->replayArgumentsAndReturnValue(handler, &savedCallinfo, &regs, this, ON_EXIT);
                    ptrace(PTRACE_SETREGS, child, 0, &regs);
                    saveArguments(handler, savedCallinfo, ON_EXIT);
                }

                replayer->addDifferencePair(!wasSame, ON_EXIT);
            } else {
                std::cerr << "Unimplemented syscall" << std::endl;
                ec = ec | UNIMPLEMENTED_SYSCALL;
            }

            replayer->nextSyscall();
        }

        handler->print();
    } else {
        std::cerr << "Unknown system state: " << (int)callinfo.op << "\n";
        ec = ec | UNDEFINED_SYSTEM_STATE;
    }
}

/**
 * @brief Checks whether differences have been encounterd during two application executions.
 * Saves all data to the protobuf objects.
 *
 * @param exitcode Exit code for sperrips process.
 */
void Tracer::finish(exit_code exitcode) {
    if (this->isRecordingMode()) {
        recorder->addASLRInfo(this->aslrInfo);
        recorder->close();
    } else {
        std::cout << "\n[i] Result: ";

        if (replayer->wasEverythingEqual()) {
            std::cout << "Both executions are identical!" << std::endl;
        } else {
            std::cout << "The executions are different!" << std::endl;
        }

        replayer->close();
    }

    exit_code realExitCode = exitcode | ec; // xor with collected exit codes

    if (realExitCode != CORRECT) {
        std::cout << "Program exited with: " << realExitCode << std::endl;

        for (int i = 0; i < 20; i++) {
            exit_code e = static_cast<exit_code>(1 << i);

            if (((int)realExitCode & (int)e) == (int)e) {
                printf("Meaning: %d => %s\n", e, Utils::getExitCodeName(e).c_str());
                fflush(stdout);
            }
        }
    }

    exit((int)realExitCode);
}

/**
 * @brief Indicates whether sperrips is operated in recording mode or not.
 *
 * @return true If sperrips operates in recording mode.
 * @return false If sperrips operates not in recording and thus in replaying mode.
 */
bool Tracer::isRecordingMode() {
    return recordingMode;
}

/**
 * @brief Reads a struct from memory.
 * Internally, it calls `readnbytes`.
 * The read length is adapted to the actual read length, returned by `readnbytes`.
 *
 * @param val Memory location to read from.
 * @param length Size of the struct.
 * @param argVal argument_value to save the data in. The struct is saved in `str` and its read length in `int64`.
 */
void Tracer::read_struct(void* val, size_t length, argument_value& argVal) {
    argVal.str = readnbytes(val, length);
    argVal.int64 = argVal.str.size(); //reset to actual read value, in case it is empty!
}

/**
 * @brief Parses the return value from \p callinfo.
 * Special handling might be required, e.g. in case of `mmap`, where the memory content of the returned addr is read from with `readnbytes`.
 * It sets the read return value within the current syscall handler, referenced by the global `handler` variable.
 *
 * @param callinfo `__ptrace_syscall_info` struct as retrieved from ptrace.
 */
void Tracer::retrieveReturnValue(__ptrace_syscall_info callinfo) {
    argument_value argVal = handler->getReturnValueAsArgument();
    uint_least64_t rval = callinfo.exit.rval;
    argument_type type = handler->getReturnType();
    argVal.type = type;

    if (type == AT_PASLR_FOLLOW) {
        size_t length = 0;

        if (handler->getType() == SYS_mmap) {
            length = handler->getArgumentValues().at(1).int64;
        }

        argVal.ptr = (void*) rval;
        argVal.str = readnbytes((void*)rval, length);
    }

    argVal.int64 = rval;
    handler->setReturnValue(argVal);
}

/**
 * @brief This method implements logic for retrieving an argument value depending on its type.
 * Depending on the argument \p type, different logic is required.
 * Either, fetching from the tracee's virtual memory area or reading from the processor registers.
 *
 * @param type Type of the argument to fetch.
 * @param callinfo Struct as retrieved via `ptrace`.
 * @param index Index number of the argument. Syscall can have up to six arguments. Hence, ranges from 0-5.
 * @param argVal The return argument to save the read data into.
 */


void Tracer::retrieveArgumentValue(argument_type type, __ptrace_syscall_info callinfo, int index, argument_value& argVal) {
    uint_least64_t val = callinfo.entry.args[index];
    argVal.type = type;

    if (type == AT_VOID_ADDR || type == AT_SIGSET_T_PTR) {
        argVal.ptr = (void*) val;
        return;
    } else if (Utils::isInt(type)) {
        argVal.int64 = val;
        return;
    } else if (type == AT_CHAR_PTR) {
        argVal.str = read_string(val);
        argVal.ptr = (void*)val;
        return;
    } else if (type == AT_OUT_BUFFER) {
        size_t length = callinfo.entry.args[index + 1];
        argVal.ptr = (void*) val;
        read_struct((void*) val, length, argVal); // we can use this function here, too, even though it is not a struct
        return;
    } else if (type == AT_VOID_PTR_FOLLOW || type == AT_PASLR_FOLLOW) {
        // parse length of sockopt from following parameter
        // some syscalls store length as socklen_t, others behind a pointer
        if (callinfo.entry.nr == SYS_getsockopt) {
            argVal.int64 = *(socklen_t*) readnbytes((void*)callinfo.entry.args[index + 1], 8).c_str();
        } else {
            argVal.int64 = callinfo.entry.args[index + 1];
        }

        size_t length = argVal.int64;
        argVal.str = readnbytes((void*) val, argVal.int64);
        argVal.ptr = (void*) val;
        return;
    } else if (type == AT_SOCKLEN_T_PTR || type == AT_UINT64_PTR) {
        // parse ptr value in str buffer, cause other code pieces access ptr values by the string buffer
        if (type == AT_SOCKLEN_T_PTR) {
            const int sizeofsocklen = sizeof(socklen_t);
            union helper {
                socklen_t val;
                char chars[sizeofsocklen];
            } sockbuf;
            sockbuf.val = *(socklen_t*) readnbytes((void*)val, 8).c_str();
            argVal.str = std::string(sockbuf.chars, sizeofsocklen);

        } else {
            const int sizeofuint64 = sizeof(uint64_t);
            union helper {
                uint64_t val;
                char chars[sizeofuint64];
            } uint64buf;
            uint64buf.val = *(uint64_t*) readnbytes((void*)val, 8).c_str();
            argVal.str = std::string(uint64buf.chars, sizeofuint64);
        }

        return;
    } else if (Utils::isStruct(type)) {
        size_t length = handler->getStructLength(type);

        if (length == -1) {
            std::cerr << "[Error]!! Seems like an uninitialized getStructLength() method for " << magic_enum::enum_name(type) << "!" << std::endl;
        }

        if (type == AT_STRUCT_SOCKADDR) {
            if (callinfo.entry.nr == SYS_getsockname || callinfo.entry.nr == SYS_getsockopt) {
                length = *(socklen_t*)(void*) readnbytes((void*)callinfo.entry.args[index + 1], 8).c_str();
            } else {
                length = callinfo.entry.args[index + 1];
            }
        }

        read_struct((void*) val, length, argVal);

        if (type == AT_STRUCT_MSGHDR) {
            struct msghdr* msghdr = (struct msghdr*) argVal.str.c_str();

            std::string msg_name = readnbytes(msghdr->msg_name, msghdr->msg_namelen);
            std::string iovecstr = readnbytes(reinterpret_cast<void*>(msghdr->msg_iov), (sizeof(struct iovec)));
            struct iovec* iovec = (struct iovec*)(iovecstr.data());

            std::string msg_control = readnbytes(msghdr->msg_control, msghdr->msg_controllen);
            std::string iovec_data = readnbytes(iovec->iov_base, iovec->iov_len);

            msc::Struct_msghdr* p_msghdr = new msc::Struct_msghdr();
            p_msghdr->set_allocated_msg_name(new std::string(msg_name));
            p_msghdr->set_allocated_msg_control(new std::string(msg_control));

            msc::Struct_iovec* p_iovec = new msc::Struct_iovec();
            p_iovec->set_allocated_iov_data(new std::string(iovec_data));

            p_msghdr->set_allocated_iovec(p_iovec);

            msc::Related* related = new msc::Related();
            related->set_allocated_msghdr(p_msghdr);
            argVal.related = (void*) related;
        }

        return;
    } else {
        std::cerr << "PROBLEM: argtype not handable!! \n";
    }

    std::cout << "val: ";
    printf("0x%08lX\n", val);
    return;
}

/**
 * @brief Returns the appropriate syscall handler, matching to the syscall opcode, given in \p callinfo.
 * It uses `callinfo.entry.nr` to determine the syscalls's opcode.
 * In case a syscall handler is found, it sets the global variable `nohandler` to false, otherwise to true.
 *
 * @param callinfo Struct as retrieved from ptrace.
 * @return std::unique_ptr<Handler> The actual syscall handler.
 */

std::unique_ptr<Handler> Tracer::getHandler(__ptrace_syscall_info callinfo) {
    uint64_t nr = callinfo.entry.nr;
    std::cout << "Call " << syscall_counter << ": ";
    std::unique_ptr<Handler> handler = std::unique_ptr<Handler>(HandlerRegistry<Handler>::create(nr));

    if (handler.get() == nullptr) {
        handler = std::unique_ptr<Handler>(new Handler());
        std::cout << "no handler for this syscall nr (" << nr << ")";
        nohandler = true;
    } else {
        nohandler = false;
        auto name = magic_enum::enum_name((msc::SyscallOpcode)handler->getType());
        std::cout << name;
    }

    std::cout << std::endl;
    return handler;
}


/**
 * @brief Checks for critical states within the tracee's execution.
 * \p policy_lax looses restrictions and accepts some more states as non criticial.
 *
 * @param policy_lax Specifies, whether some differences are accepted nonetheless (for replaying purposes)
 * @return true If the current state of sperrips (ec) does not contain a criticial state
 * @return false  If the current state of sperrips (ec) does contain a criticial state
 */

bool Tracer::checkState(bool policy_lax) {
    std::vector<exit_code> critical = {FAILED_PARSING_RECFILE, COMPARING_DIFFERENT_JOBS, DIFFERENT_SYSCALLS_ENCOUNTERED, UNEXPECTED_ARGUMENT, UNDEFINED_SYSTEM_STATE, ERROR_WRITING_TO_CHILD};
    std::vector<exit_code> lax = {POTENTIAL_MEMORY_CORRUPTION, WRONG_POINTER, UNEXPECTED_MEMORY_CONTENT, UNEXPECTED_ARGUMENT_VALUE, UNKNOWN_PARAMETER_TYPE, WRONG_INTEGER, STAT_DIFFERS, UNIMPLEMENTED_SYSCALL};

    for (int i = 0; i < critical.size(); i++) {
        exit_code e = critical.at(i);

        if (((int)ec & (int)e) == (int)e) {
            return false;
        }
    }

    if (!policy_lax) {
        for (int i = 0; i < lax.size(); i++) {
            exit_code e = lax.at(i);

            if (((int)ec & (int)e) == (int)e) {
                return false;
            }
        }
    }

    return true;
}