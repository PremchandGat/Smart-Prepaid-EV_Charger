part of 'scanner_bloc.dart';

enum ScannerStatus { scanning, scaned }

class ScannerState extends Equatable {
  final ScannerStatus status;
  final List<String> scannedData;
  final MobileScannerController cameraController;
  const ScannerState(
      {this.scannedData = const [],
      this.status = ScannerStatus.scanning,
      required this.cameraController});

  copyWith({List<String>? scannedData, ScannerStatus? status}) {
    return ScannerState(
        cameraController: cameraController,
        scannedData: scannedData ?? this.scannedData,
        status: status ?? this.status);
  }

  @override
  List<Object?> get props => [scannedData, status, cameraController];
}
