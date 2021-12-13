# Modifications

This directory includes the patch files for the applied modifications to ping and the Linux kernel.
Furthermore, the used compiled binaries are stored in `binaries/`.

## pcat
`pcat` is a small utitliy to print file content without usage of the `read` syscall, as it is affected by the kernel modification.
Therefore, `pcat` uses the similiar `pread64` syscall.

Compilation: `g++ -o pcat pcat.cpp --std=c++17`
