#!/bin/bash

kernel_version="linux-5.15.1"
url="https://cdn.kernel.org/pub/linux/kernel/v5.x/${kernel_version}.tar.xz"
signurl="https://cdn.kernel.org/pub/linux/kernel/v5.x/${kernel_version}.tar.sign"

wget "${url}"
unxz -v "${kernel_version}.tar.xz"
wget "${signurl}"

gpg --keyserver hkps://keyserver.ubuntu.com --recv-keys 647F28654894E3BD457199BE38DBBDC86092693
gpg --verify "${kernel_version}.tar.sign"

tar xvf "${kernel_version}.tar"
cd "${kernel_version}"
#cp -v /boot/config-$(uname -r) .config
read -p "Did you copy the .config file? " -n 1 -r
sed -ri '/CONFIG_SYSTEM_TRUSTED_KEYS/s/=.+/=""/g' .config
sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
#make menuconfig
read -p "Did you apply the patches?" -n 1 -r
make bindeb-pkg -j $(nproc)
