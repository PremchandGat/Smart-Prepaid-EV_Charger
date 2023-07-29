import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import 'package:mobile_scanner/mobile_scanner.dart';

part 'scanner_event.dart';
part 'scanner_state.dart';

class ScannerBloc extends Bloc<ScannerEvent, ScannerState> {
  ScannerBloc()
      : super(ScannerState(cameraController: MobileScannerController())) {
    on<ScannerEvent>((event, emit) {});
    on<UpdateScannedQrEvent>(_updaScannedData);
    on<SelecteQrCodeEvent>(_selectQrCode);
  }

  _selectQrCode(SelecteQrCodeEvent event, Emitter<ScannerState> emit) {
    emit(state.copyWith(scannedData: [event.qr]));
  }

  _updaScannedData(UpdateScannedQrEvent event, Emitter<ScannerState> emit) {
    final List<Barcode> barcodes = event.caputre.barcodes;
    List<String> codes = [];
    for (final barcode in barcodes) {
      if (!state.scannedData.contains(barcode.rawValue.toString())) {
        codes.add(barcode.rawValue.toString());
      }
    }
    emit(
        state.copyWith(scannedData: List.of(state.scannedData)..addAll(codes)));
  }
}
