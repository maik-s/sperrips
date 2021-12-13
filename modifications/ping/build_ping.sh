#!/bin/bash

thisdir="$(pwd)"
cd /tmp
git clone https://github.com/iputils/iputils.git
apt update
apt install -y --no-install-recommends \
	clang \
	docbook-xsl-ns \
	file \
	gcc \
	gettext \
	iproute2 \
	libcap-dev \
	libidn2-0-dev \
	libssl-dev \
	make \
	meson \
	pkg-config \
	xsltproc

cd iputils
patch -p1 < "${thisdir}/ping_patch.patch"
./configure
make
cp builddir/ping/ping "${thisdir}"/
cd "${thsidir}"
