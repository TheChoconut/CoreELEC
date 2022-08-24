# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libunistring"
PKG_VERSION="1.0"
PKG_SHA256="5bab55b49f75d77ed26b257997e919b693f29fd4a1bc22e0e6e024c246c72741"
PKG_LICENSE="LGPL3"
PKG_SITE="https://www.linuxfromscratch.org/blfs/view/svn/general/libunistring.html"
PKG_URL="https://ftp.gnu.org/gnu/libunistring/libunistring-1.0.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="libunistring is a library that provides functions for manipulating Unicode strings and for manipulating C strings according to the Unicode standard."

PKG_CONFIGURE_OPTS_TARGET="--disable-static"