//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <popup_master_window/popup_master_window_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) popup_master_window_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "PopupMasterWindowPlugin");
  popup_master_window_plugin_register_with_registrar(popup_master_window_registrar);
}
