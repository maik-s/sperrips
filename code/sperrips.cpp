#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sys/user.h>
#include <vector>
#include <sys/syscall.h>
#include <sys/personality.h>
#include <string>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include "Tracer.h"
#include "libs/json.hpp"
#include <sched.h>
#include <sys/mount.h>
#include <cassert>
#include "Utils.h"
#include <regex>

std::string decodeHex(std::string hex) {
    int len = hex.length();
    std::string newString;

    for (int i = 0; i < len; i += 2) {
        std::string byte = hex.substr(i, 2);
        char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }

    return newString;
}

int main(int argc, char* argv[]) {
    std::ifstream input("config.json");
    nlohmann::json json = nlohmann::json::parse(input);
    bool recordingMode = json["recording"];

    if (argc > 1) {
        if (strcmp(argv[1], "--recording") == 0) {
            recordingMode = true;
        } else if (strcmp(argv[1], "--replaying") == 0) {
            recordingMode = false;
        }
    }

    std::string jobName = json["command"];
    bool lax_policy = true;

    if (json.find("lax_policy") != json.end()) {
        lax_policy = json.at("lax_policy");
    } else {
        std::cout << "key_not_found" << std::endl;
    }

    auto list = json["commands"][jobName];
    std::cout << "Running \"" << jobName << "\" with parametes: " << list << std::endl;
    std::string mod = recordingMode ? "recording" : "replaying";
    printf("Modus: %s\n", mod.c_str());
    printf("Policy: %d\n", lax_policy);
    std::cout << std::flush;
    std::string pathPrefix = "";

    if (json["container"]["enable_container"]) {
        pathPrefix += (std::string) json["container"]["ol_upperdir"];
        int res = unshare(CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS);
        perror("unshare");
    }

    pathPrefix += "/" + (std::string) json["workdir"];
    pathPrefix = std::regex_replace(pathPrefix, std::regex("//"), "/"); // remove duplicate slashes

    errno = 0; // close(2) sets errno=einval

    if (json["container"]["enable_container"]) {
        std::cout << "Starting container" << std::endl;
        std::string mount_point = json["container"]["mount_point"];
        mount("none", "/", NULL, MS_REC | MS_PRIVATE, NULL);
        std::string mnt_params = "lowerdir=" + (std::string)json["container"]["ol_lowerdir"] + ",upperdir=" + (std::string)json["container"]["ol_upperdir"] + ",workdir=" + (std::string)json["container"]["ol_workdir"] + "";
        mount("none", mount_point.c_str(), "overlay", 0, mnt_params.c_str());
        mount("proc", (mount_point + "/proc").c_str(), "proc", 0, NULL);
        mount("udev", (mount_point + "/dev").c_str(), "devtmpfs", 0, NULL);
        mount("sysfs", (mount_point + "/sys").c_str(), "sysfs", 0, NULL);
        const char* new_root = mount_point.c_str();
        const char* old_root = (mount_point + "/mnt").c_str(); // /mnt must exists in the mounten file system, e.g. a bootstrapped debian
        int res2 = syscall(SYS_pivot_root, new_root, old_root);
        perror("pivot_root");

        if (errno != 0) {
            exit(errno);
        }

        umount2("/mnt/", MNT_DETACH);
        sethostname("sperrips", 9);
    } else {
        std::cout << "Container disabled" << std::endl;
    }

    chdir(((std::string)json["workdir"]).c_str());
    std::string stdin_msg = list["stdin"];

    if (list["is_hex"] == true) {
        stdin_msg = decodeHex(stdin_msg);
    }

    auto stdout_filepath = (std::string)json["workdir"] + "/" + (std::string)json["stdout_file"];
    auto stderr_filepath = (std::string)json["workdir"] + "/" + (std::string)json["stderr_file"];
    auto tracer = Tracer(list["args"], stdin_msg, recordingMode, json["verbosity"], jobName, stdout_filepath, stderr_filepath, pathPrefix);
    auto status = tracer.wait();
    tracer.ptracef(PTRACE_SETOPTIONS, 0, (void*)(PTRACE_O_EXITKILL | PTRACE_O_TRACESYSGOOD));
    tracer.ptracef(PTRACE_SYSCALL, NULL, NULL);
    status = tracer.wait();

    while (WIFSTOPPED(status)) {
        int sig = WSTOPSIG(status);

        if (sig == 11) {
            std::cerr << "SEGFAULT happend in child. Exiting" << std::endl;
            exit(sig);
        }

        if (!tracer.checkState(lax_policy)) {
            tracer.finish(tracer.ec);
        }

        tracer.handleSyscall();
        tracer.ptracef(PTRACE_SYSCALL, NULL, NULL);
        status = tracer.wait();
    }

    tracer.wait();
    tracer.finish(CORRECT);
    return 0;
}
