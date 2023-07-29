import 'package:evcs/core/constants/language/english.dart';
import 'package:evcs/features/scanner/bloc/scanner_bloc.dart';
import 'package:evcs/features/scanner/widget/scanner.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class QrCodeScanner extends StatelessWidget {
  const QrCodeScanner({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text(Enlang.scanQr),
      ),
      body: BlocListener<ScannerBloc, ScannerState>(
        listenWhen: (previous, current) =>
            previous.scannedData != current.scannedData,
        listener: (context, state) {
          if (state.scannedData.isNotEmpty) {
            print("Going back...............");
            //Navigator.pop(context);
          }
        },
        child: const ScannerWidget(),
      ),
    );
  }
}
