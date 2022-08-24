# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libpsl"
PKG_VERSION="0.21.1"
PKG_SHA256="ac6ce1e1fbd4d0254c4ddb9d37f1fa99dec83619c1253328155206b896210d4c"
PKG_LICENSE="BSD 2-Clause 'Simplified'"
PKG_SITE="https://www.linuxfromscratch.org/blfs/view/svn/basicnet/libpsl.html"
PKG_URL="https://github.com/rockdaboot/libpsl/releases/download/${PKG_VERSION}/libpsl-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libidn2"
PKG_LONGDESC="libpsl is a library for accessing and resolving information from the Public Suffix List (PSL)."