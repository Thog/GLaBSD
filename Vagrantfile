# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty32"
  config.vm.synced_folder "./", "/home/vagrant/project"
  config.vm.hostname = "GLaDSD"
  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y libgmp3-dev libmpfr-dev libisl-dev libcloog-isl-dev libmpc-dev texinfo make bison flex gcc g++ nasm build-essential grub zip xorriso
    export PREFIX="/opt/cross"
    export TARGET=i686-elf
    export PATH="$PREFIX/bin:$PATH"
    sudo mkdir /tmp/
    sudo mkdir /tmp/build
    cd /tmp/build
    sudo wget -O binutils.tar.gz http://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz
    tar -xf binutils.tar.gz
    cd binutils-2.25
    mkdir build-binutils
    cd build-binutils
    ../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install
    cd ../..
    rm -r binutils-2.25
    wget -O gcc.tar.gz http://ftp.gnu.org/gnu/gcc/gcc-4.9.2/gcc-4.9.2.tar.gz
    tar -xf gcc.tar.gz
    cd gcc-4.9.2
    mkdir build-gcc
    cd build-gcc
    ../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
    rm -rf /tmp/build
    sudo sed -e 's|PATH="\(.*\)"|PATH="/opt/cross/bin:\1"|g' -i /etc/environment
  SHELL
end