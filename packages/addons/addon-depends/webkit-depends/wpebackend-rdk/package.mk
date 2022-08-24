# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wpebackend-rdk"
PKG_VERSION="303ad5e70f6d2129411fa78edacf51d5272c1c5f"
PKG_SHA256="28510892486d0d4c7ebd67e2c0fa94884bd4a024fbb08fd60760c660caafc13a"
PKG_LICENSE="BSD 2-Clause 'Simplified'"
PKG_SITE="https://github.com/WebPlatformForEmbedded/WPEBackend-rdk"
PKG_URL="https://github.com/WebPlatformForEmbedded/WPEBackend-rdk/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain ${OPENGLES} libwpe libinput libxkbcommon glib"
PKG_LONGDESC="The WPEBackend-rdk library"
PKG_CMAKE_OPTS_TARGET="-DUSE_BACKEND_AML=ON"