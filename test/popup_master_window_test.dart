import 'package:flutter/src/widgets/framework.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:popup_master_window/popup_master_window.dart';
import 'package:popup_master_window/popup_master_window_platform_interface.dart';
import 'package:popup_master_window/popup_master_window_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockPopupMasterWindowPlatform
    with MockPlatformInterfaceMixin
    implements PopupMasterWindowPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');




  void main() {
    final PopupMasterWindowPlatform initialPlatform = PopupMasterWindowPlatform
        .instance;

    test('$MethodChannelPopupMasterWindow is the default instance', () {
      expect(initialPlatform, isInstanceOf<MethodChannelPopupMasterWindow>());
    });

    test('getPlatformVersion', () async {
      PopupMasterWindow popupMasterWindowPlugin = PopupMasterWindow();
      MockPopupMasterWindowPlatform fakePlatform = MockPopupMasterWindowPlatform();
      PopupMasterWindowPlatform.instance = fakePlatform;

      expect(await popupMasterWindowPlugin.getPlatformVersion(), '42');
    });
  }



  @override
  Future<bool> closePopupWindow() {
    // TODO: implement closePopupWindow
    throw UnimplementedError();
  }

  @override
  Future<bool> createPopupWindow({required String title, required int width, required int height, required Widget content, String? routeName}) {
    // TODO: implement createPopupWindow
    throw UnimplementedError();
  }






}