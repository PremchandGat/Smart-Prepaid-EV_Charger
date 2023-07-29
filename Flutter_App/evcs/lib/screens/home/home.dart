import 'package:evcs/core/widgets/gauges/gauge_widget.dart';
import 'package:evcs/features/mqtt_data/UI/mqtt_live_view.dart';
import 'package:evcs/features/scanner/bloc/scanner_bloc.dart';
import 'package:evcs/screens/scan_qr/scan_qr.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Home Page"),
      ),
      floatingActionButton: FloatingActionButton.extended(
        onPressed: () => Navigator.push(context,
            MaterialPageRoute(builder: (context) => const QrCodeScanner())),
        label: const Text("Scan"),
        icon: const Icon(Icons.qr_code),
      ),
      body: Column(
        children: [
          context.watch<ScannerBloc>().state.scannedData.isNotEmpty
              ? MqttLiveView(
                  topic: context.watch<ScannerBloc>().state.scannedData[0])
              : Expanded(
                  child: ListView(
                  children: [GaugeWidget()],
                ))
        ],
      ),
    );
  }
}
