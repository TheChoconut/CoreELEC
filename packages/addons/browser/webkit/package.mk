# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="webkit"
PKG_VERSION="1.0"
PKG_REV="101"
PKG_ARCH="arm"
PKG_LICENSE="LGPL"
PKG_SITE="https://wpewebkit.org/"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain cog wpebackend-rdk"
PKG_SECTION="browser"
PKG_TOOLCHAIN="manual"

PKG_ADDON_BROKEN="WebKit is a cross-platform web browser engine"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="WebKit"
PKG_ADDON_TYPE="xbmc.python.script"
PKG_ADDON_PROVIDES="executable"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -P $(get_install_dir cog)/usr/bin/{cog,cogctl} ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -P $(get_install_dir wpewebkit)/usr/libexec/wpe-webkit-1.1/{WPENetworkProcess,WPEWebProcess} ${ADDON_BUILD}/${PKG_ADDON_ID}/bin

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
    cp -PL $(get_install_dir cog)/usr/lib/{libcogcore.so,libcogcore.so.9,libcogcore.so.9.0.3} \
           $(get_install_dir wpewebkit)/usr/lib/{libWPEWebKit-1.1.so.0,wpe-webkit-1.1/injected-bundle/libWPEInjectedBundle.so} \
           $(get_install_dir cairo)/usr/lib/{libcairo-gobject.so.2,libcairo.so.2} \
           $(get_install_dir libwpe)/usr/lib/{libwpe-1.0.so,libwpe-1.0.so.1} \
           $(get_install_dir libsoup)/usr/lib/{libsoup-3.0.so,libsoup-3.0.so.0} \
           $(get_install_dir libepoxy)/usr/lib/{libepoxy.so,libepoxy.so.0} \
           $(get_install_dir icu)/usr/lib/{libicuuc.so,libicuuc.so.71,libicui18n.so,libicui18n.so.71,libicudata.so.71} \
           $(get_install_dir libpsl)/usr/lib/libpsl.so.5 \
           $(get_install_dir harfbuzz-icu)/usr/lib/{libharfbuzz-icu.so.0,libharfbuzz-icu.so} \
           $(get_install_dir wpebackend-rdk)/usr/lib/libWPEBackend-default.so \
           $(get_install_dir gstreamer)/usr/lib/libgstreamer-1.0.so.0 \
           ${SYSROOT_PREFIX}/usr/lib/{libgstapp-1.0.so.0,libgstbase-1.0.so.0,libgstpbutils-1.0.so.0,libgstaudio-1.0.so.0,libgsttag-1.0.so.0,libgstvideo-1.0.so.0,libgstfft-1.0.so.0,libunistring.so.2} \
           ${ADDON_BUILD}/${PKG_ADDON_ID}/lib

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib/modules
    cp -PL $(get_install_dir glib-networking)/usr/lib/gio/modules/{libgioenvironmentproxy.so,libgioopenssl.so} \
           ${ADDON_BUILD}/${PKG_ADDON_ID}/lib/modules
}