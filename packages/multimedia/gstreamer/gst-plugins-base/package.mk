# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="gst-plugins-base"
PKG_VERSION="1.20.3"
PKG_SHA256="7e30b3dd81a70380ff7554f998471d6996ff76bbe6fc5447096f851e24473c9f"
PKG_LICENSE="GPL-2.1-or-later"
PKG_SITE="https://gstreamer.freedesktop.org/modules/gst-plugins-base.html"
PKG_URL="https://gstreamer.freedesktop.org/src/gst-plugins-base/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain gstreamer"
PKG_LONGDESC="Base GStreamer plugins and helper libraries"
PKG_BUILD_FLAGS="-gold"

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Dgl=disabled \
                         -Dadder=disabled \
                         -Dapp=enabled \
                         -Daudioconvert=enabled \
                         -Daudiomixer=disabled \
                         -Daudiorate=enabled \
                         -Daudioresample=enabled \
                         -Daudiotestsrc=disabled \
                         -Dcompositor=disabled \
                         -Dencoding=disabled \
                         -Dgio=enabled \
                         -Dgio-typefinder=enabled \
                         -Doverlaycomposition=disabled \
                         -Dpbtypes=disabled \
                         -Dplayback=enabled \
                         -Drawparse=enabled \
                         -Dsubparse=enabled \
                         -Dtcp=disabled \
                         -Dtypefind=enabled \
                         -Dvideoconvert=enabled \
                         -Dvideorate=disabled \
                         -Dvideoscale=disabled \
                         -Dvideotestsrc=disabled \
                         -Dvolume=enabled \
                         -Dalsa=enabled \
                         -Dcdparanoia=disabled \
                         -Dlibvisual=disabled \
                         -Dogg=disabled \
                         -Dopus=disabled \
                         -Dpango=disabled \
                         -Dtheora=disabled \
                         -Dtremor=disabled \
                         -Dvorbis=disabled \
                         -Dx11=disabled \
                         -Dxshm=disabled \
                         -Dxvideo=disabled \
                         -Dexamples=disabled \
                         -Dtests=disabled \
                         -Dtools=disabled \
                         -Dintrospection=disabled \
                         -Dnls=disabled \
                         -Dorc=disabled \
                         -Dgobject-cast-checks=disabled \
                         -Dglib-asserts=disabled \
                         -Dglib-checks=disabled \
                         -Dpackage-name=gst-plugins-base \
                         -Dpackage-origin=LibreELEC.tv \
                         -Ddoc=disabled"
}