#include "RecordReplay.h"
#include "Utils.h" // for return type equivalence classes

/**
 * @brief Adds a syscall with the data from the given syscall handler to the protobuf object \p syscall.
 *
 * @param handler Reference to the current syscall handler.
 * @param syscall Syscall object to which the data are added.
 * @param onEntry Indicates whether the syscall's interception happend on entry or on exit.
 */

void RecordReplay::sycallhandler2ProtoSyscall(Handler* handler, msc::Syscall* syscall, bool onEntry) {
    int type = handler->getType();
    syscall->set_type((msc::SyscallOpcode)type);
    syscall->set_isexitcall(handler->isExitCall());
    msc::ArgumentList* argList = new msc::ArgumentList();

    std::vector<argument_value> argVals = handler->getArgumentValues();

    for (int i = 0; i < argVals.size(); i++) {
        argument_value aval = argVals.at(i);
        argument_type type = aval.type;
        msc::Argument* arg = argList->add_args();

        arg->set_type((msc::argument_type)type);

        if (Utils::isInt(type) || type == AT_IGNORE) {
            arg->set_int64(aval.int64);
        } else if (type == AT_CHAR_PTR) {
            std::string buffer = aval.str;
            arg->set_buffer(std::string(buffer.c_str(), buffer.size()));
            arg->set_ptr((uint64_t) aval.ptr);
        } else if (type == AT_VOID_ADDR || type == AT_UINT64_PTR || type == AT_SIGSET_T_PTR || type == AT_PASLR_FOLLOW) {
            arg->set_ptr((uint64_t)(aval.ptr));
        } else if (Utils::isStruct(type) || type == AT_OUT_BUFFER || Utils::isPointer(type)) {
            arg->set_buffer(std::string(aval.str.c_str(), aval.str.size()));
            arg->set_int64(aval.str.length());
            arg->set_ptr((uint64_t) aval.ptr);
        } else {
            std::cerr << "[Error]: Cannot serialize argument type !! " << std::endl;
        }

        if (aval.related != nullptr) {
            msc::Related* related_data = (msc::Related*) aval.related;
            arg->set_allocated_related(related_data);
        }
    }

    if (onEntry) {
        syscall->set_allocated_args_on_entry(argList);
    } else {
        syscall->set_allocated_args_on_exit(argList);
    }

    msc::Argument* returnValue = new msc::Argument();
    returnValue->set_type((msc::argument_type) handler->getReturnType());

    argument_value rValue = handler->getReturnValue();
    returnValue->set_int64(rValue.int64);

    switch (handler->getReturnType()) {
    case AT_SIGNED_INT:
        returnValue->set_int64((signed long) rValue.int64);

    case AT_PASLR_FOLLOW:
        returnValue->set_ptr((uint64_t)rValue.ptr);
        returnValue->set_buffer(rValue.str);

    default:
        break;
    }

    syscall->set_allocated_return_value(returnValue);
}

/**
 * @brief Sets system information obtained via `uname` syscall to \p sysinfo.
 *
 * @param sysinfo Reference of the object to which the obtained data are added.
 */
void RecordReplay::setSystemInformation(msc::SystemInfo* sysinfo) {
    struct utsname buf = {};
    uname(&buf);
    sysinfo->set_sysname(buf.sysname);
    sysinfo->set_nodename(buf.nodename);
    sysinfo->set_release(buf.release);
    sysinfo->set_version(buf.version);
    sysinfo->set_machine(buf.machine);
    sysinfo->set_domainname(buf.domainname);
}