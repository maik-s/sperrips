#include "Recorder.h"

/**
 * @brief Construct a new Recorder:: Recorder object and crafts a new recording protobuf object.
 *
 * @param jobname name of the sperrips job to run as specified in `config.json`.
 */

Recorder::Recorder(std::string jobname, std::string pathPrefix) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    msc::Metadata* md = new msc::Metadata(); // protobuf deletes it ! https://stackoverflow.com/a/33995232/2835625
    md->set_version(GIT_COMMIT);
    std::time_t timestamp = std::time(0);
    md->set_time_recorded(timestamp);
    md->set_job_name(jobname);
    msc::SystemInfo* sysinfo = md->mutable_sysinfo();
    setSystemInformation(sysinfo);
    recording.set_allocated_metadata(md);
    numberOfSyscalls = 0;
    this->pathPrefix = pathPrefix;
}


/**
 * @brief Add syscall to protobuf object.
 *
 * @param handler Reference to the current syscall handler.
 * @param onEntry Indicates whether the syscall's interception happend on entry or on exit.
 */
void Recorder::addSyscall(Handler* handler, bool onEntry) {
    if (onEntry) {
        currentSyscall = recording.add_syscall();
        numberOfSyscalls += 1;
    }

    RecordReplay::sycallhandler2ProtoSyscall(handler, currentSyscall, onEntry);
}

/**
 * @brief Adds the given command to the protobuf object.
 *
 * @param args Arguments of the executed job.
 * @param stdin Stdin input of the executed job.
 */

void Recorder::addCommand(std::vector<std::string> args, std::string stdin) {
    msc::Metadata* md = recording.mutable_metadata();
    msc::Command* cmd = new msc::Command();

    for (int i = 0; i < args.size(); i++) {
        cmd->add_argv(args.at(i).c_str());
    }

    cmd->set_stdin(stdin);
    md->set_allocated_command(cmd);
}

/**
 * @brief Writes the protobuf object into a file.
 *
 * @return true No error occurred during serialization.
 * @return false An error occurred during serialization.
 */

bool Recorder::close() {
    msc::Metadata* md = recording.mutable_metadata();
    md->set_number_of_syscalls(numberOfSyscalls);
    std::fstream output(RECORDING_FILENAME, std::ios::out | std::ios::trunc | std::ios::binary);
    std::string path = std::regex_replace(pathPrefix + RECORDING_FILENAME, std::regex("//"), "/"); // remove duplicate slashes

    if (!recording.SerializeToOstream(&output)) {
        std::cerr << "Failed to serialize recording" << std::endl;
        return 0;
    } else {
        std::cout << "Successfully written recording to '" << path << "'" << std::endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 1;
}

/**
 * @brief Add obtained process layout to protobuf object.
 *
 * @param aslrinfo Data to add to protobuf object.
 */
void Recorder::addASLRInfo(msc::ASLRInfo* aslrinfo) {
    recording.mutable_metadata()->set_allocated_aslrinfo(aslrinfo);
}