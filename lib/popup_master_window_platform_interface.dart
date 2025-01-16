import 'package:flutter/material.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'popup_master_window_method_channel.dart';

abstract class PopupMasterWindowPlatform extends PlatformInterface {
  PopupMasterWindowPlatform() : super(token: _token);

  static final Object _token = Object();
  static PopupMasterWindowPlatform _instance = MethodChannelPopupMasterWindow();

  static PopupMasterWindowPlatform get instance => _instance;

  static set instance(PopupMasterWindowPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<bool> createPopupWindow({
    required String title,
    required int width,
    required int height,
    required Widget content,
  }) {
    throw UnimplementedError('createPopupWindow() has not been implemented.');
  }

  Future<bool> closePopupWindow() {
    throw UnimplementedError('closePopupWindow() has not been implemented.');
  }
}
