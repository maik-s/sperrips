# ifndef MSC_REPLAYER_H
# define MSC_REPLAYER_H

#include <string>
#include <vector>
#include "handler/Handler.h"
#include "RecordReplay.h"
#include "proto_files/cpp_out/Syscall.pb.h"
#include "TypeDefs.h"
#include "mapper/ASLRMapper.h"
#include <sys/ptrace.h>
#include "Tracer.h"
#include <regex>

class Replayer: public RecordReplay {
private:
    bool recordMode;
    int numberOfSyscalls;
    int currentSyscall;
    bool everythingEqual = true;
    msc::Recording recording;
    const std::string DIFFERENCE_FILENAME = "differences.bin";
    msc::Differences differences;
    msc::Syscall* currentReplayedSyscall; // needed to add both arguments before and after
    bool _differenceOccured = false; // state for one syscall. Saves if diff occured before call
    std::string pathPrefix = "";

public:
    Replayer(std::string jobname, std::string pathPrefix);
    ~Replayer();
    std::unique_ptr<ASLRMapper> pointerMapper;
    void nextSyscall();
    void readRecordings();
    bool isCorrectType(int type);
    exit_code compareArgumentsAndReturnValue(std::unique_ptr<Handler>& handler, bool compareReturn, bool onEntry);
    exit_code replayArgumentsAndReturnValue(std::unique_ptr<Handler>& handler, struct __ptrace_syscall_info* callinfo, user_regs_struct* regs, Tracer* tracer, bool onEntry);
    bool wasEverythingEqual();
    void addDifferencePair(bool diffOccurred, bool onEntry);
    void addArguments(std::unique_ptr<Handler>& handler, bool onEntry);
    bool close();
    msc::Syscall getCurrentSyscall();
    void setReturnValue(std::unique_ptr<Handler>& handler);
};
#endif