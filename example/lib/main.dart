import 'package:flutter/material.dart';
import 'package:popup_master_window/popup_master_window.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Popup Window Example',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MainScreen(),
    );
  }
}

class MainScreen extends StatefulWidget {
  const MainScreen({super.key});

  @override
  State<MainScreen> createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  final _popupMasterWindow = PopupMasterWindow();
  String _platformVersion = 'Unknown';

  @override
  void initState() {
    super.initState();
    _getPlatformVersion();
  }

  Future<void> _getPlatformVersion() async {
    final version = await _popupMasterWindow.getPlatformVersion();
    if (!mounted) return;

    setState(() {
      _platformVersion = version ?? 'Unknown';
    });
  }

  Future<void> _handleCloseWindow() async {
    try {
      final success = await _popupMasterWindow.closePopupWindow();
      if (success && mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('پنجره با موفقیت بسته شد')),
        );
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('خطا در بستن پنجره: $e')),
        );
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('پلاگین پنجره جدید'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () {
                debugPrint('نسخه ویندوز: $_platformVersion');
              },
              child: const Text("نسخه ویندوز"),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () async {
                try {
                  final success = await _popupMasterWindow.createPopupWindow(
                    title: 'پنجره جدید',
                    width: 800,
                    height: 600,
                    content: popupContent(popupMasterWindow: _popupMasterWindow),
                  );

                    // استفاده از push به جای pushReplacement
                   await Navigator.of(context).pushReplacement(MaterialPageRoute(
                      builder: (context) =>  popupContent(popupMasterWindow: _popupMasterWindow),
                    ));

                } catch (e) {
                  if (mounted) {
                    ScaffoldMessenger.of(context).showSnackBar(
                      SnackBar(content: Text('خطا در باز کردن پنجره: $e')),
                    );
                  }
                }
              },
              child: const Text('باز کردن پنجره جدید'),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: _handleCloseWindow,
              child: const Text('بستن پنجره'),
            ),
          ],
        ),
      ),
    );
  }
}

class popupContent extends StatefulWidget {
  PopupMasterWindow popupMasterWindow;
   popupContent({super.key, required this.popupMasterWindow});

  @override
  State<popupContent> createState() => _popupContentState();
}

class _popupContentState extends State<popupContent> {


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.deepOrangeAccent,
      appBar: AppBar(
        title: const Text('پنجره جدید'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () {
                debugPrint('این یک دکمه در پنجره جدید است');
              },
              child: const Text("دکمه پنجره جدید"),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () async {
                await widget. popupMasterWindow.closePopupWindow();
                if (mounted) {
                  // استفاده از maybePop به جای pop مستقیم
                  Navigator.of(context).maybePop();
                }
              },
              child: const Text('بستن پنجره جدید'),
            ),
          ],
        ),
      ),
    );
  }
}
