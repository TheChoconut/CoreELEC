/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-amlvsink
 *
 * FIXME:Describe amlvsink here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! amlvsink ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <gst/gst.h>
#include <stdio.h>
#include "gstamlvsink.h"

GST_DEBUG_CATEGORY_STATIC(gst_aml_vsink_debug);
#define GST_CAT_DEFAULT gst_aml_vsink_debug
#define VERSION "1.1"

#define COMMON_VIDEO_CAPS        \
  "width = (int) [ 16, 4096 ], " \
  "height = (int) [ 16, 4096 ] "

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE("sink",
                                                                   GST_PAD_SINK,
                                                                   GST_PAD_ALWAYS,
                                                                   GST_STATIC_CAPS(
                                                                       "video/mpeg, "
                                                                       "mpegversion = (int) { 1, 2, 4 }, "
                                                                       "systemstream = (boolean) false, " COMMON_VIDEO_CAPS "; "
                                                                       "video/x-h264, "
                                                                       "stream-format={ byte-stream }, "
                                                                       "alignment={ nal }, " COMMON_VIDEO_CAPS "; "
                                                                       "video/x-h265, "
                                                                       "stream-format={ byte-stream }, "
                                                                       "alignment={ nal };"
                                                                       "video/x-divx, " COMMON_VIDEO_CAPS "; "
                                                                       "video/x-vp9, " COMMON_VIDEO_CAPS "; "
                                                                       "video/x-cavs;"
                                                                       "video/x-msmpeg, " COMMON_VIDEO_CAPS "; "
                                                                       "image/jpeg, " COMMON_VIDEO_CAPS "; "
                                                                       "video/x-raw, "
                                                                       "format = (string) { YUY2, I420, YV12, UYVY, AYUV, GRAY8, BGR, RGB }," COMMON_VIDEO_CAPS "; "
                                                                       //        "video/x-pn-realvideo; "
                                                                       "video/x-wmv, "
                                                                       "wmvversion = (int) { 1, 3 }, " COMMON_VIDEO_CAPS));

static void gst_aml_vsink_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gst_aml_vsink_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void gst_aml_vsink_finalize(GObject *obj);

static GstFlowReturn gst_aml_vsink_render(GstBaseSink *parent, GstBuffer *buffer);
static gboolean gst_aml_vsink_event(GstBaseSink *parent, GstEvent *event);
static gboolean gst_aml_vsink_query(GstBaseSink *parent, GstQuery *query);
static gboolean gst_aml_vsink_setcaps(GstBaseSink *parent, GstCaps *caps);

static gboolean gst_aml_vsink_start(GstBaseSink *vsink);
static gboolean gst_aml_vsink_stop(GstBaseSink *vsink);

#define gst_aml_vsink_parent_class parent_class
G_DEFINE_TYPE(GstAmlVsink, gst_aml_vsink, GST_TYPE_BASE_SINK);

static void gst_aml_vsink_class_init(GstAmlVsinkClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS(klass);
  GstBaseSinkClass *gstbasesink_class = GST_BASE_SINK_CLASS(klass);

  gobject_class->set_property = gst_aml_vsink_set_property;
  gobject_class->get_property = gst_aml_vsink_get_property;
  gobject_class->finalize = gst_aml_vsink_finalize;

  gst_element_class_set_static_metadata(gstelement_class,
                                        "Amlogic Video Sink",
                                        "Codec/Decoder/Video",
                                        "Amlogic VPU Decoder/Sink Plugin",
                                        "user@example.com");

  gst_element_class_add_static_pad_template(gstelement_class, &sink_factory);

  gstbasesink_class->start = GST_DEBUG_FUNCPTR(gst_aml_vsink_start);
  gstbasesink_class->stop = GST_DEBUG_FUNCPTR(gst_aml_vsink_stop);
  gstbasesink_class->render = GST_DEBUG_FUNCPTR(gst_aml_vsink_render);
  gstbasesink_class->preroll = GST_DEBUG_FUNCPTR(gst_aml_vsink_render);
  gstbasesink_class->event = GST_DEBUG_FUNCPTR(gst_aml_vsink_event);
  gstbasesink_class->query = GST_DEBUG_FUNCPTR(gst_aml_vsink_query);
  gstbasesink_class->set_caps = GST_DEBUG_FUNCPTR(gst_aml_vsink_setcaps);
}

static void
gst_amlvsink_polling_eos(GstAmlVsink *amlvsink)
{
  static int last_pts = -1;
  static int stop_count = 5;
  static int delay = 0;
  unsigned long pts;

  usleep(1000 * 100);

  pts = codec_get_vpts(amlvsink->pcodec);
  if (amlvsink->last_checkin_pts != -1L && pts != -1L && pts != 1)
  {
    if (last_pts != pts)
    {
      last_pts = pts;
      stop_count = delay + 5;
    }
    else if (!amlvsink->is_paused && amlvsink->is_eos)
    {
      stop_count--;
    }

    GST_INFO_OBJECT(amlvsink, "TESTV %ld %ld %d %d %d", pts,
                    amlvsink->last_checkin_pts, amlvsink->is_eos, amlvsink->is_paused,
                    stop_count);
    if ((pts > amlvsink->last_checkin_pts && amlvsink->is_eos) || (amlvsink->is_eos && !amlvsink->is_paused && stop_count < 0))
    {
      GST_BASE_SINK_CLASS(parent_class)->event(parent_class, gst_event_new_eos());
      gst_task_pause(amlvsink->eos_task);
    }
  }
  else if (pts == 1)
  {
    delay++;
  }
}

static void gst_aml_vsink_init(GstAmlVsink *vsink)
{
  // On this sink class
  // On the base class
  gst_base_sink_set_sync(GST_BASE_SINK(vsink), TRUE);
  gst_base_sink_set_max_lateness(GST_BASE_SINK(vsink), 20 * GST_MSECOND);
  gst_base_sink_set_qos_enabled(GST_BASE_SINK(vsink), TRUE);
  gst_base_sink_set_drop_out_of_segment(GST_BASE_SINK(vsink), FALSE);

  /* TODO: coords
  vsink->coordinate[0] = DEFAULT_WINDOW_X;
  vsink->coordinate[1] = DEFAULT_WINDOW_Y;
  vsink->coordinate[2] = DEFAULT_WINDOW_WIDTH;
  vsink->coordinate[3] = DEFAULT_WINDOW_HEIGHT;
  set_video_axis(vsink->coordinate); */

} // end gst_aml_vsink_init.

static void
start_eos_task(GstAmlVsink *amlvsink)
{
  if (!amlvsink->eos_task)
  {
    amlvsink->eos_task = gst_task_new(
        (GstTaskFunction)gst_amlvsink_polling_eos, amlvsink, NULL);
    gst_task_set_lock(amlvsink->eos_task, &amlvsink->eos_lock);
  }
  gst_task_start(amlvsink->eos_task);
}

static void
stop_eos_task(GstAmlVsink *amlvsink)
{
  if (!amlvsink->eos_task)
    return;
  gst_task_stop(amlvsink->eos_task);
  gst_task_join(amlvsink->eos_task);
  gst_object_unref(amlvsink->eos_task);
  amlvsink->eos_task = NULL;
}

static gboolean
gst_aml_vsink_stop(GstBaseSink *sink)
{
  GstAmlVsink *amlvsink = GST_AMLVSINK(sink);
  gint ret = 0;
  stop_eos_task(amlvsink);
  if (amlvsink->codec_init_ok)
  {
    amlvsink->codec_init_ok = 0;
    if (amlvsink->is_paused == TRUE)
    {
      ret = codec_resume(amlvsink->pcodec);
      if (ret != 0)
      {
        GST_ERROR("resume failed!ret=%d", ret);
      }
      else
      {
        amlvsink->is_paused = FALSE;
      }
    }
    set_black_policy(1);
    codec_close(amlvsink->pcodec);

    if (amlvsink->list) {
      g_list_free(amlvsink->list);
      amlvsink->list = NULL;
    }

    amlvsink->is_headerfeed = FALSE;
  }
  if (amlvsink->info)
  {
    amlvsink->info->finalize(amlvsink->info);
    amlvsink->info = NULL;
  }

  if (amlvsink->pcodec)
  {
    g_free(amlvsink->pcodec);
    amlvsink->pcodec = NULL;
  }
  return TRUE;
}

static gboolean gst_aml_vsink_start(GstBaseSink *vsink)
{
  GstAmlVsink *amlvsink = GST_AMLVSINK(vsink);

  amlvsink->pcodec = g_malloc(sizeof(codec_para_t));
  memset(amlvsink->pcodec, 0, sizeof(codec_para_t));
  amlvsink->pcodec->has_video = 1;
  amlvsink->pcodec->has_audio = 0;
  amlvsink->pcodec->am_sysinfo.width = 0;
  amlvsink->pcodec->am_sysinfo.height = 0;
  amlvsink->pcodec->am_sysinfo.rate = 0;
  amlvsink->pcodec->noblock = 0;
  amlvsink->pcodec->stream_type = STREAM_TYPE_ES_VIDEO;
  // amlvsink->pcodec->dec_mode          = STREAM_TYPE_FRAME;
  // amlvsink->pcodec->video_path        = FRAME_BASE_PATH_AMLVIDEO_AMVIDEO;
  amlvsink->is_headerfeed = FALSE;
  amlvsink->is_paused = FALSE;
  amlvsink->is_eos = FALSE;
  amlvsink->codec_init_ok = 0;

  return TRUE;

} // end gst_aml_vsink_init.

static gboolean
gst_aml_vsink_setcaps(GstBaseSink *bsink, GstCaps *vscapslist)
{
  GstAmlVsink *amlvsink;
  GstStructure *structure;
  const char *name;
  AmlStreamInfo *videoinfo = NULL;
  gint32 ret = CODEC_ERROR_NONE;

  amlvsink = GST_AMLVSINK(bsink);
  structure = gst_caps_get_structure(vscapslist, 0);
  name = gst_structure_get_name(structure);
  if (NULL == name)
  {
    return FALSE;
  }
  
  videoinfo = amlVstreamInfoInterface(name);
  amlvsink->info = videoinfo;
  if (0 != videoinfo->init(videoinfo, amlvsink->pcodec, structure))
  {
    return FALSE;
  }

  // WHAT IF WIDTH AND HEIGHT ARE ZERO?
  if (0 == amlvsink->pcodec->am_sysinfo.width || 0 == amlvsink->pcodec->am_sysinfo.height)
  {
    return FALSE;
  }

  if (!amlvsink->codec_init_ok)
  {
    ret = codec_init(amlvsink->pcodec);

    if (ret != CODEC_ERROR_NONE)
    {
      GST_ERROR("codec init failed, ret=-0x%x", -ret);
      return FALSE;
    }

    amlvsink->codec_init_ok = 1;
    // start_eos_task(amlvsink);
    GST_DEBUG_OBJECT(amlvsink, "pcodec: video codec_init ok");
  }

  return TRUE;
}

static void gst_aml_vsink_finalize(GObject *obj)
{
  G_OBJECT_CLASS(parent_class)->finalize(obj);
} // end gst_aml_vsink_finalize.

static void gst_aml_vsink_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  //  GstAmlVsink* vsink = GST_AMLVSINK (object);

  switch (prop_id)
  {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  } // end switch prop_id...
} // end gst_aml_vsink_set_property.

static void gst_aml_vsink_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  //  GstAmlVsink* vsink = GST_AMLVSINK(object);

  switch (prop_id)
  {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  } // end switch prop_id...
} // end gst_aml_vsink_get_property.

// -------------- GstBaseSink virtual methods ---------------------------------------------------------------

static GstFlowReturn gst_aml_vsink_render(GstBaseSink *parent, GstBuffer *buffer)
{
  GstFlowReturn ret = GST_FLOW_OK;
  GstAmlVsink *amlvsink = GST_AMLVSINK(parent);
  GSList *l;
  GstBuffer *p = buffer;

  amlvsink->list = g_slist_append(amlvsink->list, (gpointer)buffer);
  // If the codec wasn't initialized,
  // try to skip the frame.a
  if (!amlvsink->codec_init_ok)
    return GST_FLOW_OK;

  l = amlvsink->list;
  while (l) {
		GSList *node = l;
		l = g_slist_next(l);
		if (node->data) {
			p = (GstBuffer *) (node->data);
			ret = gst_aml_vsink_handle_frame(amlvsink, buffer);
			if (G_UNLIKELY(ret != GST_FLOW_OK)) {
				GST_ERROR_OBJECT(amlvsink, "failed gst_aml_vsink_handle_frame");
			}
		}

		amlvsink->list = g_slist_remove_link(amlvsink->list, node);
		g_slist_free_1(node);
	}
  

  
  return ret;
} // end gst_aml_vsink_render.

GstFlowReturn gst_aml_vsink_handle_frame(GstAmlVsink *amlvsink, GstBuffer *buffer)
{
  GstFlowReturn ret = GST_FLOW_OK;
  guint8 *data;
  guint size;
  gint written;
  GstClockTime timestamp = GST_CLOCK_TIME_NONE, pts;
  GstMapInfo map;
  struct buf_status vbuf;

  while (codec_get_vbuf_state(amlvsink->pcodec, &vbuf) == 0)
  {
    if (vbuf.data_len * 10 < vbuf.size * 7)
    {
      break;
    }
    if (amlvsink->is_paused)
    {
      break;
    }
    usleep(20000);
  }
  if (GST_BUFFER_PTS_IS_VALID(buffer))
    timestamp = GST_BUFFER_PTS(buffer);

  pts = timestamp * 9LL / 100000LL + 1L;

  if (timestamp != GST_CLOCK_TIME_NONE)
  {
    //GST_INFO_OBJECT(amlvsink, " video pts = %x", (unsigned long)pts);
    if (codec_checkin_pts(amlvsink->pcodec, (unsigned long)pts) != 0)
    {
      GST_ERROR_OBJECT(amlvsink, "pts checkin flied maybe lose sync");
    }
    else
    {
      amlvsink->last_checkin_pts = pts;
    }
  }

  if (!amlvsink->is_headerfeed)
  {
    if (amlvsink->info->writeheader)
    {
      amlvsink->info->writeheader(amlvsink->info, amlvsink->pcodec);
    }
    amlvsink->is_headerfeed = TRUE;
  }
  if (amlvsink->info->add_startcode)
  {
    amlvsink->info->add_startcode(amlvsink->info, amlvsink->pcodec, buffer);
  }
  if (!gst_buffer_map(buffer, &map, GST_MAP_READ))
    return GST_FLOW_ERROR;
  data = map.data;
  size = map.size;

  while (size > 0)
  {
    written = codec_write(amlvsink->pcodec, data, size);
    if (written >= 0)
    {
      size -= written;
      data += written;
    }
    else if (errno == EAGAIN || errno == EINTR)
    {
      GST_WARNING_OBJECT(amlvsink, "codec_write busy");
      if (amlvsink->is_paused)
      {
        break;
      }
      usleep(20000);
    }
    else
    {
      GST_ERROR_OBJECT(amlvsink, "codec_write failed");
      ret = GST_FLOW_ERROR;
      break;
    }
  }

  gst_buffer_unmap(buffer, &map);
  return ret;
}

static gboolean gst_aml_vsink_event(GstBaseSink *parent, GstEvent *event)
{
  gboolean ret = TRUE;
  GstAmlVsink *vsink = GST_AMLVSINK(parent);

  switch (GST_EVENT_TYPE(event))
  {
  case GST_EVENT_EOS:
    GST_WARNING("get GST_EVENT_EOS,check for video end");
    if (vsink->codec_init_ok)
    {
      gst_task_start(vsink->eos_task);
      vsink->is_eos = TRUE;
      ret = FALSE;
    }
    else
    {
      ret = GST_BASE_SINK_CLASS(parent_class)->event(parent, event);
    }

    break;
  default:
    ret = GST_BASE_SINK_CLASS(parent_class)->event(parent, event);
    break;
  }

  return ret;
} // end gst_aml_vsink_event.

static gboolean gst_aml_vsink_query(GstBaseSink *parent, GstQuery *query)
{
  gboolean ret;
  // FIXME: We may need to answer query GST_QUERY_POSITION. https://github.com/gcsuri/gst-aml-plugins1/blob/master/video/amlvsink/gstamlvsink.c
  switch (GST_QUERY_TYPE(query))
  {
  case GST_QUERY_SEEKING:
  {
    // Seeking is not supported
    GstFormat fmt;
    // gst_query_parse_seeking(query, &fmt, NULL, NULL, NULL);
    // gst_query_set_seeking(query, fmt, FALSE, 0, -1);
    ret = TRUE;
    break;
  }
  default:
    ret = GST_BASE_SINK_CLASS(parent_class)->query(parent, query);
    break;
  } // end switch query...

  return ret;
} // end gst_aml_vsink_query.

/*
static GstStateChangeReturn gst_aml_vsink_change_state(GstElement* element, GstStateChange transition)
{
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
  GstAmlVsink* vsink = GST_AMLVSINK(element);

  ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);
}//end gst_aml_vsink_change_state.*/

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
amlvsink_init(GstPlugin *amlvsink)
{
  /* debug category for fltering log messages
   */
  GST_DEBUG_CATEGORY_INIT(gst_aml_vsink_debug, "amlvsink", 0, "Amlogic Video Decoder/Sink");

  return gst_element_register(amlvsink, "amlvsink", GST_RANK_PRIMARY + 1, GST_TYPE_AMLVSINK);
}

GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    amlvsink,
    "Amlogic Video Decoder/Sink",
    amlvsink_init,
    VERSION,
    "LGPL",
    "Amlogic",
    "http://amlogic.com/")
