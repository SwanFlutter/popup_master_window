#include "include/popup_master_window/popup_master_window_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "popup_master_window_plugin.h"

void PopupMasterWindowPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  popup_master_window::PopupMasterWindowPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
