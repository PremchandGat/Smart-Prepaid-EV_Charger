part of 'scanner_bloc.dart';

@immutable
abstract class ScannerEvent extends Equatable {}

class UpdateScannerStateEvent extends ScannerEvent {
  final String data;
  final ScannerStatus status;
  UpdateScannerStateEvent({required this.data, required this.status});
  @override
  List<Object?> get props => [data, status];
}

class SelecteQrCodeEvent extends ScannerEvent {
  final String qr;
  SelecteQrCodeEvent(this.qr);
  @override
  List<Object?> get props => [qr];
}

class ToggleFlashEvent extends ScannerEvent {
  @override
  List<Object?> get props => throw UnimplementedError();
}

class UpdateScannedQrEvent extends ScannerEvent {
  final BarcodeCapture caputre;
  UpdateScannedQrEvent(this.caputre);
  @override
  List<Object?> get props => [caputre];
}
