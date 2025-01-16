import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import 'popup_master_window_platform_interface.dart';

/// An implementation of [PopupMasterWindowPlatform] that uses method channels.
class MethodChannelPopupMasterWindow extends PopupMasterWindowPlatform {
  @visibleForTesting
  final methodChannel = const MethodChannel('popup_master_window');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<bool> createPopupWindow({
    required String title,
    required int width,
    required int height,
    required Widget content,
  }) async {
    final Map<String, dynamic> arguments = {
      'title': title,
      'width': width,
      'height': height,
      'content': _encodeWidget(content),
    };

    final bool? result = await methodChannel.invokeMethod<bool>(
      'createPopupWindow',
      arguments,
    );
    return result ?? false;
  }

  @override
  Future<bool> closePopupWindow() async {
    final bool? result = await methodChannel.invokeMethod<bool>('closePopupWindow');
    return result ?? false;
  }

  // Helper function to convert Flutter Widget to a serializable format, in this case a Map
  Map<String, dynamic> _encodeWidget(Widget widget) {
    // Return a map containing widget type and its properties
    return {
      'type': widget.runtimeType.toString(),
      'properties': _serializeProperties(widget),
    };
  }

  // A function to serialize the properties of any widget
  Map<String, dynamic> _serializeProperties(Widget widget) {
    Map<String, dynamic> properties = {};
    if (widget is Text) {
      properties['data'] = widget.data;
      properties['style'] = {
        'color': widget.style?.color?.value,
        'fontSize': widget.style?.fontSize,
      };
    } else if (widget is Scaffold) {
      properties['appBar'] = {
        'title': (widget.appBar as AppBar).title.toString(),
      };
    } else if (widget is Center) {
      properties['child'] = widget.child.runtimeType.toString();
    }
    return properties;
  }
}
