
import 'package:flutter/material.dart';

import 'popup_master_window_platform_interface.dart';


class PopupMasterWindow {
  Future<String?> getPlatformVersion() {
    return PopupMasterWindowPlatform.instance.getPlatformVersion();
  }

  Future<bool> createPopupWindow({
    required String title,
    required int width,
    required int height,
    required Widget content,
  }) {
    return PopupMasterWindowPlatform.instance.createPopupWindow(
      title: title,
      width: width,
      height: height,
      content: content,
    );
  }

  Future<bool> closePopupWindow() {
    return PopupMasterWindowPlatform.instance.closePopupWindow();
  }
}