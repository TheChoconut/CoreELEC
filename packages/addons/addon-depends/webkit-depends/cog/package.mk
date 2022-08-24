# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cog"
PKG_VERSION="0.13.90"
PKG_SHA256="a7ed11a10c41a9c4ad9692730357e6d86f2544d308d96018d26f80b234e16080"
PKG_LICENSE="MIT"
PKG_SITE="https://wpewebkit.org"
PKG_URL="https://wpewebkit.org/releases/cog-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain wpewebkit"
PKG_LONGDESC="Cog is a small single window launcher for the WebKit WPE port"
PKG_CMAKE_OPTS_TARGET="-DCOG_PLATFORM_HEADLESS=OFF -DCOG_PLATFORM_WL=OFF -DCOG_PLATFORM_DRM=OFF"