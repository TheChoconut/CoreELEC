# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libsoup"
PKG_VERSION="3.0.7"
PKG_SHA256="ebdf90cf3599c11acbb6818a9d9e3fc9d2c68e56eb829b93962972683e1bf7c8"
PKG_LICENSE="BSD 2-Clause 'Simplified'"
PKG_SITE="https://libsoup.org"
PKG_URL="https://download.gnome.org/sources/libsoup/3.0/libsoup-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain nghttp2 libpsl glib glib-networking"
PKG_LONGDESC="libsoup is an HTTP client/server library for GNOME."
PKG_MESON_OPTS_TARGET="--buildtype=release -Dvapi=disabled -Dgssapi=disabled -Dsysprof=disabled -Dtests=false -Dautobahn=disabled -Dtls_check=false"