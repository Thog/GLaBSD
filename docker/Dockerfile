FROM ubuntu:14.04
MAINTAINER Thog <thog92@hotmail.fr]

# install dependencies
RUN apt-get update && \
apt-get install -y gcc wget build-essential openssh-server grub && \
apt-get clean autoclean && \
apt-get autoremove -y && \
rm -rf /var/lib/apt /var/lib/dpkg /var/lib/cache /var/lib/log

# specify binutils/gcc version
ENV DOWNLOAD_BINUTILS=binutils-2.26
ENV DOWNLOAD_GCC=gcc-4.9.3

# specify TARGET
ENV TARGET=i686-elf
ENV PREFIX=/usr/local

# Install binutils cross toolchain
RUN wget -q http://ftp.gnu.org/gnu/binutils/$DOWNLOAD_BINUTILS.tar.gz && \
tar -xzf $DOWNLOAD_BINUTILS.tar.gz && \
mkdir -p /srv/build_binutils && \
cd /srv/build_binutils && \
/$DOWNLOAD_BINUTILS/configure --target=$TARGET --prefix="$PREFIX" \
--with-sysroot --disable-nls --disable-werror && \
make && \
make install && \
rm -r /$DOWNLOAD_BINUTILS /srv/build_binutils

# Install gcc cross toolchain
RUN wget -q ftp://ftp.gnu.org/gnu/gcc/$DOWNLOAD_GCC/$DOWNLOAD_GCC.tar.gz && \
tar -xzf $DOWNLOAD_GCC.tar.gz && \
cd /$DOWNLOAD_GCC && contrib/download_prerequisites && \
mkdir -p /srv/build_gcc && \
cd /srv/build_gcc && \
/$DOWNLOAD_GCC/configure --target=$TARGET --prefix="$PREFIX" \
-disable-nls --enable-languages=c,c++ --without-headers && \
make all-gcc && \
make all-target-libgcc && \
make install-gcc && \
make install-target-libgcc && \
rm -r /$DOWNLOAD_GCC /srv/build_gcc

# Configure access
RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
RUN echo "export VISIBLE=now" >> /etc/profile

WORKDIR /
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]