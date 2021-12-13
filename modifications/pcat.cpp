// g++ -o pcat pcat.cpp -std=c++17
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>
#include <filesystem>
#include <cstdint>

int main (int a, const char** args) {
        int fd = open(args[1], O_RDONLY);
        std::uintmax_t size = std::filesystem::file_size(args[1]);
        char* buf = (char*) malloc(size);
        int ret = pread64(fd, buf, size, 0);
        write(1, buf, size);
}
