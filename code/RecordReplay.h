# ifndef MSC_RECORD_REPLAY_H
# define MSC_RECORD_REPLAY_H

#include <string>
#include "proto_files/cpp_out/Syscall.pb.h"
#include "handler/Handler.h"
#include "TypeDefs.h"
#include <sys/utsname.h>

class RecordReplay {
public:
    const std::string RECORDING_FILENAME = "recording.bin";
    static void sycallhandler2ProtoSyscall(Handler* handler, msc::Syscall* syscall, bool onEntry);
    void setSystemInformation(msc::SystemInfo* sysinfo);
};

#endif