#include "include/popup_master_window/popup_master_window_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#include "popup_master_window_plugin_private.h"

#define POPUP_MASTER_WINDOW_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), popup_master_window_plugin_get_type(), \
                              PopupMasterWindowPlugin))

struct _PopupMasterWindowPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(PopupMasterWindowPlugin, popup_master_window_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void popup_master_window_plugin_handle_method_call(
    PopupMasterWindowPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    response = get_platform_version();
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

FlMethodResponse* get_platform_version() {
  struct utsname uname_data = {};
  uname(&uname_data);
  g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
  g_autoptr(FlValue) result = fl_value_new_string(version);
  return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
}

static void popup_master_window_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(popup_master_window_plugin_parent_class)->dispose(object);
}

static void popup_master_window_plugin_class_init(PopupMasterWindowPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = popup_master_window_plugin_dispose;
}

static void popup_master_window_plugin_init(PopupMasterWindowPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  PopupMasterWindowPlugin* plugin = POPUP_MASTER_WINDOW_PLUGIN(user_data);
  popup_master_window_plugin_handle_method_call(plugin, method_call);
}

void popup_master_window_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  PopupMasterWindowPlugin* plugin = POPUP_MASTER_WINDOW_PLUGIN(
      g_object_new(popup_master_window_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "popup_master_window",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
