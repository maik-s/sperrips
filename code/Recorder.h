# ifndef MSC_RECORDER_H
# define MSC_RECORDER_H

#include <sys/ptrace.h>
#include "handler/Handler.h"
#include <string>
#include <iostream>
#include <fstream>
#include "proto_files/cpp_out/Syscall.pb.h"
#include <memory>
#include<ctime>
#include <vector>
#include "version.h"
#include "RecordReplay.h"
#include <regex>

class Recorder: public RecordReplay {
private:
    msc::Recording recording;
    int numberOfSyscalls;
    msc::Syscall* currentSyscall;
    std::string pathPrefix = "";
public:
    Recorder(std::string jobname, std::string pathPrefix);
    void addCommand(std::vector<std::string> args, std::string stdin);
    void addSyscall(Handler* handler, bool onEntry);
    void addASLRInfo(msc::ASLRInfo* aslrinfo);
    bool close();
};
#endif