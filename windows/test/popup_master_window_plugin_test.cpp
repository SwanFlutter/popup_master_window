#include <flutter/method_call.h>
#include <flutter/method_result_functions.h>
#include <flutter/standard_method_codec.h>
#include <gtest/gtest.h>
#include <windows.h>

#include <memory>
#include <string>
#include <variant>

#include "popup_master_window_plugin.h"



namespace popup_master_window {
    namespace test {

        namespace {

            using flutter::EncodableMap;
            using flutter::EncodableValue;
            using flutter::MethodCall;
            using flutter::MethodResultFunctions;

            class MockRegistrar : public flutter::PluginRegistrarWindows {
            public:
                MockRegistrar() : flutter::PluginRegistrarWindows(nullptr) {}
                ~MockRegistrar() override = default;
            };

        }  // namespace

        TEST(PopupMasterWindowPlugin, GetPlatformVersion) {
        MockRegistrar registrar;
        PopupMasterWindowPlugin plugin(&registrar);
        // Save the reply value from the success callback.
        std::string result_string;
        plugin.HandleMethodCall(
                MethodCall("getPlatformVersion", std::make_unique<EncodableValue>()),
                std::make_unique<MethodResultFunctions<>>(
                [&result_string](const EncodableValue* result) {
                    result_string = std::get<std::string>(*result);
                },
                nullptr, nullptr));

        // Since the exact string varies by host, just ensure that it's a string
        // with the expected format.
        EXPECT_TRUE(result_string.rfind("Windows ", 0) == 0);
    }

}  // namespace test
}
