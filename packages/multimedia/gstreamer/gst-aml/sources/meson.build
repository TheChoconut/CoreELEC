project('gst-aml', 'c', version : '1.19.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()

api_version = '1.0'
sysroot = meson.get_external_property('root')

gst_dep = dependency('gstreamer-1.0', version : '>=1.19',
    required : true, fallback : ['gstreamer', 'gst_dep'])
gstbase_dep = dependency('gstreamer-base-1.0', version : '>=1.19',
  fallback : ['gstreamer', 'gst_base_dep'])

plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-template-plugin')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer template Plug-ins')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cdata)

gstvideo_dep = dependency('gstreamer-video-1.0',
    fallback: ['gst-plugins-base', 'video_dep'])

libamcodec_dep = cc.find_library('libamcodec', required: true)

found = false
foreach idir : ['/local/include/amcodec', '/include/amcodec', '/include/']
  inc_arg = '-I' + sysroot + idir
  if not found and cc.has_header('codec.h', args: inc_arg)
    plugin_c_args += [inc_arg]
    found = true
  endif
endforeach

# Plugin 1
plugin_sources = [
  'src/video/amlsysctl/gstamlsysctl.c',
  'src/video/amstreaminfo/amlstreaminfo.c',
  'src/video/amlvdec/amlvideoinfo.c',
  'src/video/amlvdec/gstamlvdec.c',
]

gstpluginexample = library('gstamlvdec',
  plugin_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstvideo_dep, libamcodec_dep],
  install : true,
  install_dir : plugins_install_dir,
)

plugin2_sources = [
  'src/video/amlsysctl/gstamlsysctl.c',
  'src/video/amstreaminfo/amlstreaminfo.c',
  'src/video/amlvdec/amlvideoinfo.c',
  'src/video/amlvdec/gstamlvsink.c',
]

gstpluginexample = library('gstamlvsink',
  plugin2_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstvideo_dep, libamcodec_dep],
  install : true,
  install_dir : plugins_install_dir,
)