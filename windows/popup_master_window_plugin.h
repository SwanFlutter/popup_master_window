#ifndef FLUTTER_PLUGIN_POPUP_MASTER_WINDOW_PLUGIN_H_
#define FLUTTER_PLUGIN_POPUP_MASTER_WINDOW_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

#include <flutter/binary_messenger.h>
#include <flutter/standard_method_codec.h>
#include <string>
#include <windows.h>

namespace popup_master_window {

    class PopupMasterWindowPlugin : public flutter::Plugin {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        PopupMasterWindowPlugin(flutter::PluginRegistrarWindows* registrar);
        virtual ~PopupMasterWindowPlugin();

        // Disallow copy and assign
        PopupMasterWindowPlugin(const PopupMasterWindowPlugin&) = delete;
        PopupMasterWindowPlugin& operator=(const PopupMasterWindowPlugin&) = delete;

        void HandleMethodCall(
                const flutter::MethodCall<flutter::EncodableValue> &method_call,
                std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    private:
        // Window management
        HWND popup_window_;
        HWND flutter_view_;
        flutter::PluginRegistrarWindows* registrar_;
        std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;

        // Window ID management
        static std::map<HWND, std::string> window_ids_;
        std::string GenerateUniqueId();

        // Window procedures and utilities
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool CreatePopupWindow(const std::string& title, int width, int height, const flutter::EncodableMap& content);
        void ClosePopupWindow();
        static std::wstring Utf8ToWide(const std::string& str);

        // Window state tracking
        bool IsValidWindow() const { return popup_window_ && IsWindow(popup_window_); }
        void CleanupWindowResources();
        void ReparentFlutterView();

        // Window creation helpers
        bool RegisterWindowClass();
        bool SetupChildWindow(int width, int height);
        void ConfigurePopupWindow();
    };

}  // namespace popup_master_window

#endif  // FLUTTER_PLUGIN_POPUP_MASTER_WINDOW_PLUGIN_H_