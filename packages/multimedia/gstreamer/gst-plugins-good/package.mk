# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="gst-plugins-good"
PKG_VERSION="1.20.3"
PKG_SHA256="f8f3c206bf5cdabc00953920b47b3575af0ef15e9f871c0b6966f6d0aa5868b7"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://gstreamer.freedesktop.org/modules/gst-plugins-good.html"
PKG_URL="https://gstreamer.freedesktop.org/src/gst-plugins-good/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain gst-plugins-base"
PKG_LONGDESC="GStreamer Good Plug-ins is a set of plug-ins that we consider to have good quality code, correct functionality, our preferred license."

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Dv4l2=disabled \
						 -Dexamples=disabled \
						 -Dtests=disabled \
						 -Dnls=disabled \
						 -Dximagesrc=disabled \
						 -Drpicamsrc=disabled \
						 -Dgtk3=disabled \
						 -Djpeg=disabled \
						 -Dpng=disabled \
						 -Dpulse=disabled \
						 -Dshout2=disabled \
						 -Dqt5=disabled \
						 -Ddirectsound=disabled \
						 -Ddv1394=disabled \
						 -Dgdk-pixbuf=disabled \
						 -Djack=disabled \
						 -Dlibcaca=disabled \
						 -Dy4m=disabled \
						 -Dflac=disabled \
						 -Dwavparse=disabled \
                         -Dpackage-name=gst-plugins-good \
                         -Dpackage-origin=LibreELEC.tv \
                         -Ddoc=disabled"
}

post_makeinstall_target() {
  # clean up
  #safe_remove ${INSTALL}/usr/bin
  #safe_remove ${INSTALL}/usr/share
}
