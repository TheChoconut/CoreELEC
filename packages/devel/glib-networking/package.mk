# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="glib-networking"
PKG_VERSION="2.72.2"
PKG_SHA256="cd2a084c7bb91d78e849fb55d40e472f6d8f6862cddc9f12c39149359ba18268"
PKG_LICENSE="LGPL"
PKG_SITE="https://www.linuxfromscratch.org/blfs/view/svn/basicnet/glib-networking.html"
PKG_URL="https://download.gnome.org/sources/glib-networking/2.72/glib-networking-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain glib openssl"
PKG_LONGDESC="The GLib Networking package contains Network related gio modules for GLib."
PKG_TOOLCHAIN="meson"

PKG_MESON_OPTS_TARGET="--buildtype=release \
					   -Dgnutls=disabled \
					   -Dopenssl=enabled \
					   -Dinstalled_tests=false \
                       -Ddefault_library=static"

post_makeinstall_target() {
  :
}
