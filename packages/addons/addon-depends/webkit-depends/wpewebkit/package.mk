# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wpewebkit"
PKG_VERSION="2.36.6"
PKG_SHA256="e98a4eae2464ffaf4e5b53be08b9ae6386db511015c8002918b824d29f05d58f"
PKG_LICENSE="MIT"
PKG_SITE="https://wpewebkit.org"
PKG_URL="https://wpewebkit.org/releases/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain cairo harfbuzz-icu systemd icu libjpeg-turbo libepoxy libpng sqlite zlib libwebp libgcrypt libxml2 libwpe libsoup gst-plugins-base "
PKG_LONGDESC="WebKit is a cross-platform web browser engine"
PKG_TOOLCHAIN="cmake"
PKG_CMAKE_OPTS_TARGET="-DPORT=WPE \
					   -DCMAKE_BUILD_TYPE=Release \
					   -DUSE_GSTREAMER_GL=OFF \
					   -DUSE_GSTREAMER_HOLEPUNCH=ON \
					   -DUSE_WOFF2=OFF \
					   -DENABLE_ACCESSIBILITY=OFF \
					   -DENABLE_WEBDRIVER=OFF \
					   -DENABLE_WEB_CRYPTO=OFF \
					   -DENABLE_XSLT=OFF \
					   -DENABLE_BUBBLEWRAP_SANDBOX=OFF \
					   -DUSE_LCMS=OFF \
					   -DUSE_OPENJPEG=OFF"