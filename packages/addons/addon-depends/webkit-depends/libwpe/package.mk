# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libwpe"
PKG_VERSION="d8b7324fbdf1e25a34a0ac601fcc2264ba0fa704"
PKG_SHA256="0f70379471a3f893306a0976e935a43799201dbf61d25c767838e32bcc9fcd93"
PKG_LICENSE="BSD 2-Clause 'Simplified'"
PKG_SITE="https://github.com/WebPlatformForEmbedded/libwpe"
PKG_URL="https://github.com/WebPlatformForEmbedded/libwpe/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libxkbcommon"
PKG_LONGDESC="General-purpose library specifically developed for the WPE-flavored port of WebKit."
PKG_MESON_OPTS_TARGET="-Dbuild-docs=false"