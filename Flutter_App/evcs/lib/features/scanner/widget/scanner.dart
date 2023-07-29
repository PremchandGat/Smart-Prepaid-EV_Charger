import 'package:evcs/core/constants/language/english.dart';
import 'package:evcs/features/scanner/bloc/scanner_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:mobile_scanner/mobile_scanner.dart';

class ScannerWidget extends StatelessWidget {
  const ScannerWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<ScannerBloc, ScannerState>(builder: (context, state) {
      return Stack(alignment: Alignment.center, children: [
        MobileScanner(
          controller: state.cameraController,
          onDetect: (capture) =>
              context.read<ScannerBloc>().add(UpdateScannedQrEvent(capture)),
        ),
        Positioned(
            child: Column(
          children: [
            for (String i in state.scannedData)
              FilledButton.tonal(
                  onPressed: () {
                    context.read<ScannerBloc>().add(SelecteQrCodeEvent(i));
                    Navigator.pop(context);
                  },
                  child: Text(i))
          ],
        )),
        Positioned(
            bottom: MediaQuery.of(context).size.width * 0.2,
            child: FilledButton.icon(
                label: const Text(Enlang.flash),
                onPressed: () => state.cameraController.toggleTorch(),
                icon: ValueListenableBuilder(
                  builder: (BuildContext context, value, Widget? child) {
                    switch (value) {
                      case TorchState.on:
                        {
                          return const Icon(Icons.flash_off);
                        }
                      default:
                        return const Icon(Icons.flash_on);
                    }
                  },
                  valueListenable: state.cameraController.torchState,
                ))),
      ]);
    });
  }
}
