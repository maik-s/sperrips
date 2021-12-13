#include "Replayer.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdint> // reinterpret_cast<std::uintptr_t>(p)
#include "comparer/IntegerComparer.h"
#include "comparer/BufferComparer.h"
#include "comparer/StructStatComparer.h"
#include "libs/magic_enum.hpp"
#include "Utils.h" // for return type equivalence classes

/**
 * @brief Construct a new Replayer:: Replayer object and crafts new protobufs objects.
 *
 * @param jobname Name of the job to run, as specified in the `config.json` file.
 */

Replayer::Replayer(std::string jobname, std::string pathPrefix) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    std::fstream input(RECORDING_FILENAME, std::ios::in | std::ios::binary);
    std::string path = std::regex_replace(pathPrefix + RECORDING_FILENAME, std::regex("//"), "/"); // remove duplicate slashes

    if (!recording.ParseFromIstream(&input)) {
        std::cerr << "ERROR: Failed to parse recording from '" << path << "'." << std::endl;
        exit(FAILED_PARSING_RECFILE);
    } else {
        msc::Metadata md = recording.metadata();
        numberOfSyscalls = md.number_of_syscalls();

        if (md.job_name() != jobname) {
            std::cerr << "ERROR: You compare different jobs! Exiting" << std::endl;
            exit(COMPARING_DIFFERENT_JOBS);
        }

        std::cout << std::flush;
        std::cout << "[Replayer] Successfully read " << numberOfSyscalls << " syscalls from '" << path << "' recording." << std::endl;
    }

    currentSyscall = 0;
    pointerMapper = std::make_unique<ASLRMapper>();
    msc::Metadata* md = new msc::Metadata();
    md->CopyFrom(recording.metadata());   // copy saved metadata and replace some information
    md->set_version(GIT_COMMIT);
    std::time_t timestamp = std::time(0);
    md->set_time_recorded(timestamp);
    md->set_job_name(jobname);
    msc::SystemInfo* sysinfo = md->mutable_sysinfo();
    setSystemInformation(sysinfo);
    differences.set_allocated_metadata(md);
    this->pathPrefix = pathPrefix;
}

Replayer::~Replayer() {
    google::protobuf::ShutdownProtobufLibrary();
}

/**
 * @brief Increases the internal syscall counter to point to the next recorded syscall.
 *
 */

void Replayer::nextSyscall() {
    if (currentSyscall < numberOfSyscalls) {
        msc::Syscall syscall = recording.syscall(currentSyscall);
        currentSyscall++;
    }
}

/**
 * @brief Returns the current recorded syscall.
 *
 * @return msc::Syscall Current recorded syscall.
 */

msc::Syscall Replayer::getCurrentSyscall() {
    assert(currentSyscall < numberOfSyscalls);
    return recording.syscall(currentSyscall);
}

/**
 * @brief Checks whether the current syscall's type matches \p type.
 *
 * @param type Syscall type to check for, given as its opcode.
 * @return true If type of current recorded syscall matches \p type.
 * @return false If type of current recorded syscall does not match \p type.
 */


bool Replayer::isCorrectType(int type) {
    auto reced = magic_enum::enum_name(this->getCurrentSyscall().type());
    auto repled = magic_enum::enum_name((msc::SyscallOpcode) type);
    return this->getCurrentSyscall().type() == type || this->getCurrentSyscall().type() == -1; // make exception, if recorded syscall was unknown
}



/**
 * @brief Compares whether arguments and return value of an intercepted syscall match to the recored one.
 * The return is a XORed value of all occurred exit_code.
 * However, it may contain accaptable differences.
 * Thus, meaning of the exit_code must be checked on usage.
 *
 *
 * @param handler Current syscall handler.
 * @param compareReturn True, if returnvalue should also be compared
 * @param onEntry Indicates whether the check happens on entry or on exit of a syscall.
 * @return exit_code XOR of all encountered error code. On success it returns CORRECT.
 */

exit_code Replayer::compareArgumentsAndReturnValue(std::unique_ptr<Handler>& handler, bool compareReturn, bool onEntry) {
    std::vector<argument_value> argValues = handler->getArgumentValues();
    bool same = true;
    exit_code ret = CORRECT;

    msc::ArgumentList argList;

    if (onEntry) {
        assert(argValues.size() == this->getCurrentSyscall().args_on_entry().args_size());
        argList = this->getCurrentSyscall().args_on_entry();
    } else {
        assert(argValues.size() == this->getCurrentSyscall().args_on_exit().args_size());
        argList = this->getCurrentSyscall().args_on_exit();
    }

    std::unique_ptr<IntegerComparer> intcmp = std::make_unique<IntegerComparer>();
    std::unique_ptr<BufferComparer> buffcmp = std::make_unique<BufferComparer>();

    for (int i = 0; i < argValues.size(); i++) {
        if (onEntry && Utils::isAfterCall(argValues.at(i).type)) {
            continue;
        }

        msc::Argument savedArg = argList.args(i);

        if (savedArg.type() != (msc::argument_type) argValues.at(i).type) {
            auto expArg = magic_enum::enum_name(savedArg.type());
            auto actual = magic_enum::enum_name((msc::argument_type) argValues.at(i).type);
            std::cout << "[Error]: Expected arg type: " << expArg << ", actual: " << actual << std::endl;
            ret = ret | UNEXPECTED_ARGUMENT;
        }

        argument_value val = argValues.at(i);

        if (val.type == AT_VOID_ADDR  || val.type == AT_UINT64_PTR || val.type == AT_SIGSET_T_PTR || val.type == AT_PASLR_FOLLOW) {
            void* recordedPointer = reinterpret_cast<void*>(savedArg.ptr());
            void* replayedPointer = val.ptr;

            bool map = pointerMapper->map(recordedPointer, replayedPointer);

            if (!map) {
                ret = (ret | WRONG_POINTER);
            }

            same = same && map;
        } else if (Utils::isInt(val.type)) {
            bool intCompare = intcmp->compare(savedArg.int64(), val.int64);
            same = same && intCompare;

            if (!intCompare) {
                ret = ret | WRONG_INTEGER;
            }

            // printf("Utils::isInt same: %d, %ld <-> %ld\n", same,savedArg.int64(), val.int64);
        } else if (val.type == AT_CHAR_PTR) {
            same = same && buffcmp->compare(savedArg.buffer(), val.str);
            bool map = pointerMapper->map((void*) savedArg.ptr(), val.ptr);

            if (!map) {
                ret = (ret | WRONG_POINTER);
            }

            // printf("AT_CHAR_PTR same: %d , '%s' <-> '%s'\n", same, savedArg.buffer().c_str(), val.str.c_str());
        } else if (Utils::isStruct(val.type) || val.type == AT_OUT_BUFFER || val.type == AT_VOID_PTR_FOLLOW || val.type == AT_SOCKLEN_T_PTR || val.type == AT_PASLR_FOLLOW) {
            bool map = pointerMapper->map((void*) savedArg.ptr(), val.ptr);

            if (!map) {
                ret = (ret | WRONG_POINTER);
            }

            if (val.type == AT_STRUCT_STAT) {
                std::unique_ptr<StructStatComparer> strstcmpr = std::make_unique<StructStatComparer>();
                struct stat* replayed = (struct stat*) val.str.c_str();
                struct stat* recorded = (struct stat*) savedArg.buffer().c_str();
                bool statEqual = strstcmpr->compare(recorded, replayed) && intcmp->compare(savedArg.int64(), val.int64);
                same = same && statEqual;

                if (!statEqual) {
                    ret = ret | STAT_DIFFERS;
                }
            } else {
                bool buffEqual = buffcmp->compare(savedArg.buffer(), val.str);  // check for bytewise equality, applies to struct if no specific comparer has been implemented
                same = same && buffEqual;

                if (!buffEqual) {
                    if (val.type == AT_STRUCT_UTSNAME) {
                        ret = ret | UNAME_DIFFERENT;
                    } else {
                        ret = ret | UNEXPECTED_MEMORY_CONTENT;
                    }

                    assert(intcmp->compare(savedArg.int64(), val.str.size()));
                }
            }
        } else {
            fflush(stdout);
            std::cerr << "ERROR: Unknown parameter type !!" << std::endl;
            ret = ret | UNKNOWN_PARAMETER_TYPE;
        }
    }

    if (compareReturn) {
        argument_type returntype = handler->getReturnType();

        if (Utils::isInt(returntype)) {
            std::unique_ptr<IntegerComparer> intcmp = std::make_unique<IntegerComparer>();
            same = same && intcmp->compare(this->getCurrentSyscall().return_value().int64(), handler->getReturnValue().int64);
        } else if (returntype == AT_VOID_ADDR || returntype == AT_PASLR_FOLLOW) {
            void* recordedRV = reinterpret_cast<void*>(this->getCurrentSyscall().return_value().ptr());
            void* replayedRV = reinterpret_cast<void*>(handler->getReturnValue().ptr);
            bool map = pointerMapper->map(recordedRV, replayedRV);

            if (!map) {
                fflush(stdout);
                ret = (ret | WRONG_POINTER);
            }

            same = same && map;

            if (returntype == AT_PASLR_FOLLOW) {
                bool buff = buffcmp->compare(this->getCurrentSyscall().return_value().buffer(), handler->getReturnValue().str);
                same = same && buff;

                if (!buff) {
                    fflush(stdout);
                    std::cerr << "[Error] Memory contains unexpected content. Exiting.." << std::endl;
                    fflush(stderr);
                    ret = (ret | UNEXPECTED_MEMORY_CONTENT);
                }
            }
        } else {
            fprintf(stderr, "ERROR: unknown return type \"%s\"!\n", ((std::string) magic_enum::enum_name(returntype)).c_str());
        }
    }

    everythingEqual = everythingEqual && same;
    printf("Parameters equal (check rval: %s): %s\n", compareReturn ? "true" : "false", same ? "true" : "false");
    return ret;
}


/**
 * @brief Replays arguments in the context of the tracee. It either manipulates processor registers or the tracee's virtual memory.
 *
 * @param handler The current syscall handler, used to retrieve argument values.
 * @param callinfo Struct, as returned by ptrace.
 * @param regs Struct of current processor register values. May be alterted by the function.
 * @param tracer Reference to the tracer class, to call some methods.
 * @param onEntry Indicates whether the syscall interception happend onentry or onexit.
 * @return exit_code
 */
exit_code Replayer::replayArgumentsAndReturnValue(std::unique_ptr<Handler>& handler, struct __ptrace_syscall_info* callinfo, user_regs_struct* regs, Tracer* tracer, bool onEntry) {
    printf("[i] Replaying arguments\n");
    msc::ArgumentList argList;
    exit_code ret = CORRECT;

    if (onEntry) {
        argList = this->getCurrentSyscall().args_on_entry();
    } else {
        argList = this->getCurrentSyscall().args_on_exit();
    }

    std::vector<argument_type> argValues = handler->getArgumentTypes();

    for (int i = 0; i < argValues.size(); i++) {
        argument_type type = argValues.at(i);
        msc::Argument savedArg = argList.args(i);
        bool success = false;
        bool didSomething = false;

        if (Utils::isInt(type) || type == AT_VOID_ADDR || type == AT_UINT64_PTR_PTR || type == AT_SIGSET_T_PTR || type == AT_PASLR_FOLLOW) {  // modify atom data like numbers directly in the registers, before the call is executed and the params passed
            uint64_t val = savedArg.int64();

            if (type == AT_VOID_ADDR || type == AT_UINT64_PTR_PTR || type == AT_SIGSET_T_PTR || type == AT_PASLR_FOLLOW) {
                // pointers are just numbers, hence threat them like it
                val = reinterpret_cast<uint64_t>(pointerMapper->getMapping(reinterpret_cast<void*>(savedArg.ptr())));
            }

            switch (i) {
            case RDI:
                regs->rdi = val;
                break;

            case RSI:
                regs->rsi = val;
                break;

            case RDX:
                regs->rdx = val;
                break;

            case RCX:
                regs->rcx = val;
                break;

            case R8:
                regs->r8 = val;
                break;

            case R9:
                regs->r9 = val;
                break;

            default:
                fprintf(stderr, "ERROR: Unknown register!!");
                break;
            }

            if (callinfo->entry.nr == SYS_write && i == 2) {
                // libc_write checks if write syscall printed as much as initially passed
                regs->rbx = val;
            }

            handler->getArgumentValues().at(i).int64 = val;
            callinfo->entry.args[i] = val; // This does not have any effect on the logic at the child, it's only that the resulted print statements show correct values.
            printf("[i] Replayed atom %s: %ld\n", ((std::string) magic_enum::enum_name(type)).c_str(), val);
        }

        if (Utils::isStruct(type) || type == AT_VOID_PTR_FOLLOW || type == AT_OUT_BUFFER || type == AT_CHAR_PTR || type == AT_SOCKLEN_T_PTR || type == AT_UINT64_PTR || type == AT_PASLR_FOLLOW) {
            void* addr = (void*) callinfo->entry.args[i];
            std::string raw_struct = std::string(savedArg.buffer());
            size_t oldSize = handler->getArgumentValues().at(i).str.size();

            if (type == AT_STRUCT_SIGACTION || type == AT_STRUCT_MSGHDR) {
                // update embedded addresses
                std::string currentStruct = tracer->readnbytes(addr, sizeof(struct msghdr));
                raw_struct = handler->updateStructAdresses(type, raw_struct, currentStruct, pointerMapper.get());
            }

            exit_code succ = tracer->write_to_child(reinterpret_cast<unsigned long>(addr), raw_struct, oldSize);
            success = success && (succ == CORRECT);

            if (succ == CORRECT) {
                std::string after = tracer->readnbytes(addr, savedArg.buffer().size());
                assert(after.compare(raw_struct) == 0);
            } else {
                return succ;
            }

            printf("[i] Patched child @ %p, success: %s\n", (void*) addr, (succ == CORRECT) ? "true" : "false");
            didSomething = true;
            fflush(stdout);

            if ((enum argument_type) savedArg.type() == AT_STRUCT_MSGHDR) {
                struct msghdr* msghdr = (struct msghdr*) raw_struct.c_str();
                uint64_t msgname = reinterpret_cast<unsigned long>(msghdr->msg_name);
                uint64_t msgcontrol = reinterpret_cast<unsigned long>(msghdr->msg_control);
                std::string iovecaddr = tracer->readnbytes(msghdr->msg_iov, 8);

                union addr {
                    char chars [8];
                    unsigned long addr;
                } a;
                memcpy(a.chars, iovecaddr.data(), 8);
                uint64_t msgiovec = reinterpret_cast<unsigned long>(a.addr);
                exit_code succ = tracer->write_to_child(msgname, savedArg.related().msghdr().msg_name(), msghdr->msg_namelen);
                ret = ret | succ;
                success = success && (succ == CORRECT);
                printf("[i] Patched related %s @ %p, success: %s\n", "msgname", (void*) msgname, (succ == CORRECT) ? "true" : "false");
                fflush(stdout);
                succ = tracer->write_to_child(msgcontrol, savedArg.related().msghdr().msg_control(), msghdr->msg_controllen);
                ret = ret | succ;
                success = success && (succ == CORRECT);
                printf("[i] Patched related %s @ %p, success: %s\n", "msgcontrol", (void*) msgcontrol, (succ == CORRECT) ? "true" : "false");
                fflush(stdout);
                succ = tracer->write_to_child(msgiovec, savedArg.related().msghdr().iovec().iov_data(), savedArg.related().msghdr().iovec().iov_data().size());
                ret = ret | succ;
                success = success && (succ == CORRECT);
                printf("[i] Patched related %s @ %p, success: %s\n", "msgiovec", reinterpret_cast<void*>(msgiovec), (succ == CORRECT) ? "true" : "false");
                fflush(stdout);

                if (succ != CORRECT) {
                    return succ;
                }
            }
        }

        if (didSomething) {
            if (success) {
                printf("[i] Replayed %s: %p\n", ((std::string) magic_enum::enum_name(type)).c_str(), (void*) callinfo->entry.args[i]);
            }
        }
    }

    if (!onEntry) {
        argument_type rValType = handler->getReturnType();
        argument_value rval = handler->getReturnValue();
        uint64_t oldRV = this->getCurrentSyscall().return_value().int64();
        uint64_t newRV = rval.int64; // defaults to current

        if (Utils::isInt(rValType)) {
            if (newRV != oldRV) {
                newRV = oldRV;
                printf("[i] Patched return value from %ld to %ld\n", handler->getReturnValue().int64, oldRV);
            }
        } else if (Utils::isPointer(rValType)) {

            void* replayedAddr = (void*) handler->getReturnValue().int64;
            oldRV = this->getCurrentSyscall().return_value().ptr();
            bool map = pointerMapper->map((void*) oldRV, replayedAddr);

            if (!map) {
                void* mappedPointer = pointerMapper->getMapping((void*) oldRV);
                rval.ptr = mappedPointer;
                newRV = (uint64_t) mappedPointer;
                printf("[i] Patched return value from %p to %p\n", (void*) replayedAddr, mappedPointer);
                ret = ret | WRONG_POINTER;
            }
        }

        regs->rax = newRV;
        rval.int64 = newRV;
        handler->setReturnValue(rval);
    }

    return ret;
}

/**
 * @brief Indicates whether application execution happend equal.
 * The value is influenced by checks within `compareArgumentsAndReturnValue`.
 *
 * @return true If everything was equal.
 * @return false If differences occurred.
 */
bool Replayer::wasEverythingEqual() {
    return everythingEqual;
}

/**
 * @brief Adds a difference to the protobuf object.
 * The data to add are obtained from the recorded data and the current syscall.
 *
 * @param diffOccurred Indicate whether a differenced occured.
 * @param onEntry Indicates whether it happend on entry or on exit.
 */

void Replayer::addDifferencePair(bool diffOccurred, bool onEntry) {
    if (currentReplayedSyscall == nullptr) {
        return;
    }

    if (onEntry) {
        this->_differenceOccured = diffOccurred;
        return;
    }

    if (this->_differenceOccured || diffOccurred) {
        msc::DiffPair* diffpair = differences.add_differences();
        msc::Syscall* recorded = new msc::Syscall();
        recorded->CopyFrom(this->getCurrentSyscall());
        diffpair->set_allocated_recorded(recorded);
        diffpair->set_allocated_replayed(currentReplayedSyscall);
    }

    this->_differenceOccured = false;
}

/**
 * @brief Adds argument values to the protobuf object of the current syscall.
 *
 * @param handler Reference to handler.
 * @param onEntry Indicates whether the syscall is intercepted on entry or on exit.
 */

void Replayer::addArguments(std::unique_ptr<Handler>& handler, bool onEntry) {
    if (onEntry) {
        currentReplayedSyscall = new msc::Syscall();
    }

    assert(currentReplayedSyscall != nullptr);
    RecordReplay::sycallhandler2ProtoSyscall(handler.get(), currentReplayedSyscall, onEntry);
}

/**
 * @brief Sets the return value for the current syscall within the protobuf object.
 *
 * @param handler Reference to the current syscall handler. Used to obtained the return value via `getReturnValue()`.
 */

void Replayer::setReturnValue(std::unique_ptr<Handler>& handler) {
    msc::Argument* arg = new msc::Argument();
    arg->set_type((msc::argument_type) handler->getReturnType());
    arg->set_int64(handler->getReturnValue().int64);
    arg->set_buffer(handler->getReturnValue().str);
    arg->set_ptr((uint64_t) handler->getReturnValue().ptr);

    if (differences.differences_size() > 0) {
        currentReplayedSyscall->set_allocated_return_value(arg);
    }
}

/**
 * @brief Writes the protobuf object into a file.
 *
 * @return true No error during serialization occurred.
 * @return false An error during serialization occurred.
 */

bool Replayer::close() {
    if (this->_differenceOccured) {
        // if have pending difference. This is the case if a difference occurred on entry and the traceer have been aborted then.
        this->addDifferencePair(true, false);
    }

    msc::Metadata* md = differences.mutable_metadata();
    md->set_number_of_syscalls(numberOfSyscalls);
    md->set_number_of_differences(differences.differences().size());
    differences.set_equal(this->wasEverythingEqual());
    std::fstream output(DIFFERENCE_FILENAME, std::ios::out | std::ios::trunc | std::ios::binary);
    std::string path = std::regex_replace(pathPrefix + DIFFERENCE_FILENAME, std::regex("//"), "/"); // remove duplicate slashes

    if (!differences.SerializeToOstream(&output)) {
        std::cerr << "ERROR: Failed to serialize differences" << std::endl;
        return 0;
    } else {
        std::cout << "Successfully written differences to '" << path << "'" << std::endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 1;
}
