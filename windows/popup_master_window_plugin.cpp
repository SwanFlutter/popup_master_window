#include "popup_master_window_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#include <flutter/standard_method_codec.h>
#include <VersionHelpers.h>
#include <map>

#include <shobjidl.h>
#include <dwmapi.h>
#include <windowsx.h>

namespace popup_master_window {

    void PopupMasterWindowPlugin::RegisterWithRegistrar(
            flutter::PluginRegistrarWindows *registrar) {
        auto channel =
                std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
                        registrar->messenger(), "popup_master_window",
                                &flutter::StandardMethodCodec::GetInstance());

        auto plugin = std::make_unique<PopupMasterWindowPlugin>(registrar);

        channel->SetMethodCallHandler(
                [plugin_pointer = plugin.get()](const auto &call, auto result) {
                    plugin_pointer->HandleMethodCall(call, std::move(result));
                });

        registrar->AddPlugin(std::move(plugin));
    }

    PopupMasterWindowPlugin::PopupMasterWindowPlugin(flutter::PluginRegistrarWindows* registrar)
            : popup_window_(nullptr),
              registrar_(registrar),
              flutter_view_(nullptr) {}

    PopupMasterWindowPlugin::~PopupMasterWindowPlugin() {
        if (popup_window_) {
            ClosePopupWindow();
        }
    }

    std::wstring PopupMasterWindowPlugin::Utf8ToWide(const std::string& str) {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }

    LRESULT CALLBACK PopupMasterWindowPlugin::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
    DestroyWindow(hwnd);  // به جای مخفی کردن، پنجره را نابود می‌کنیم
    return 0;

    case WM_DESTROY:
    // حذف PostQuitMessage
    return 0;

    default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
}

bool PopupMasterWindowPlugin::CreatePopupWindow(
        const std::string& title,
        int width,
        int height,
        const flutter::EncodableMap& content) {

    if (popup_window_) {
        ClosePopupWindow();  // Close any existing popup before creating a new one
    }

    WNDCLASSEXW window_class = {};
    window_class.cbSize = sizeof(WNDCLASSEXW);
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.lpszClassName = L"PopupMasterWindow";

    RegisterClassExW(&window_class);

    DWORD window_style = WS_OVERLAPPEDWINDOW;
    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, window_style, FALSE);

    std::wstring wide_title = Utf8ToWide(title);

    popup_window_ = CreateWindowExW(
            0,
            window_class.lpszClassName,
            wide_title.c_str(),
            window_style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left, rect.bottom - rect.top,
            nullptr,
            nullptr,
            window_class.hInstance,
            nullptr
    );

    if (!popup_window_) {
        return false;
    }

    // Get the Flutter window handle from the registrar
    flutter_view_ = registrar_->GetView()->GetNativeWindow();

    // Create a child window handle for Flutter content
    HWND child_window = CreateWindowEx(
            0,
            L"STATIC",
            L"",
            WS_CHILD | WS_VISIBLE,
            0, 0, width, height,
            popup_window_,
            nullptr,
            GetModuleHandle(nullptr),
            nullptr);

    // Reparent the Flutter view
    SetParent(flutter_view_, child_window);

    // Position and show the Flutter view
    SetWindowPos(
            flutter_view_,
            nullptr,
            0, 0,
            width, height,
            SWP_SHOWWINDOW);

    ShowWindow(popup_window_, SW_SHOW);
    UpdateWindow(popup_window_);

    if (content.size() > 0) {
        // Extract information from the content
        auto type_iter = content.find(flutter::EncodableValue("type"));
        if (type_iter != content.end() && std::holds_alternative<std::string>(type_iter->second)) {
            std::string type = std::get<std::string>(type_iter->second);
            // Here you check the type of the content
            OutputDebugStringA(("Received widget type: " + type).c_str());

            // Access widget properties
            auto properties_iter = content.find(flutter::EncodableValue("properties"));
            if (properties_iter != content.end() && std::holds_alternative<flutter::EncodableMap>(properties_iter->second)) {
                auto properties = std::get<flutter::EncodableMap>(properties_iter->second);

                // Print all the properties of the Widget.
                for (const auto& pair : properties){
                    if (std::holds_alternative<std::string>(pair.second)) {
                        OutputDebugStringA(("Property " +  std::get<std::string>(pair.first) +  ": "+std::get<std::string>(pair.second)).c_str());
                    }else if (std::holds_alternative<int>(pair.second)) {
                        OutputDebugStringA(("Property " +  std::get<std::string>(pair.first) +  ": "+std::to_string(std::get<int>(pair.second))).c_str());
                    }else if (std::holds_alternative<double>(pair.second)) {
                        OutputDebugStringA(("Property " +  std::get<std::string>(pair.first) +  ": "+std::to_string(std::get<double>(pair.second))).c_str());
                    }
                }
            }
        }
    }

    return true;
}


void PopupMasterWindowPlugin::ClosePopupWindow() {
    if (popup_window_) {
        // برگرداندن flutter_view به پنجره اصلی
        if (flutter_view_ && IsWindow(flutter_view_)) {
            HWND main_window = GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT);
            SetParent(flutter_view_, main_window);
        }

        // بستن پنجره پاپ‌آپ
        DestroyWindow(popup_window_);
        popup_window_ = nullptr;

        // پاکسازی پیام‌های باقی‌مانده
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void PopupMasterWindowPlugin::HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (method_call.method_name().compare("createPopupWindow") == 0) {
        const auto* arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
        if (arguments) {
            auto title = std::get<std::string>(arguments->at(flutter::EncodableValue("title")));
            auto width = std::get<int>(arguments->at(flutter::EncodableValue("width")));
            auto height = std::get<int>(arguments->at(flutter::EncodableValue("height")));
            const auto content_iter = arguments->find(flutter::EncodableValue("content"));
            if(content_iter == arguments->end()) {
                result->Error("INVALID_ARGUMENTS", "Content not found for createPopupWindow");
                return;
            }

            const auto* content  = std::get_if<flutter::EncodableMap>(&content_iter->second);
            if(!content){
                result->Error("INVALID_ARGUMENTS", "Invalid content for createPopupWindow");
                return;
            }

            bool success = CreatePopupWindow(title, width, height, *content);
            result->Success(flutter::EncodableValue(success));
        } else {
            result->Error("INVALID_ARGUMENTS", "Invalid arguments for createPopupWindow");
        }
    }
    else if (method_call.method_name().compare("closePopupWindow") == 0) {
        ClosePopupWindow();
        result->Success(flutter::EncodableValue(true));
    }
    else if (method_call.method_name().compare("getPlatformVersion") == 0) {
        std::ostringstream version_stream;
        version_stream << "Windows ";
        if (IsWindows10OrGreater()) {
            version_stream << "10+";
        } else if (IsWindows8OrGreater()) {
            version_stream << "8";
        } else if (IsWindows7OrGreater()) {
            version_stream << "7";
        }
        result->Success(flutter::EncodableValue(version_stream.str()));
    } else {
        result->NotImplemented();
    }
}

}  // namespace popup_master_window