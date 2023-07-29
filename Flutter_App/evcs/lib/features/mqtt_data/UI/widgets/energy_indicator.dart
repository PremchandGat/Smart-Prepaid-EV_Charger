import 'package:evcs/features/mqtt_data/bloc/mqtt_client_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class EnergyIndicatorWidget extends StatelessWidget {
  const EnergyIndicatorWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MqttClientBloc, MqttClientState>(
        builder: (context, state) {
      return Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text("Energy Consumed: ${state.data.energy ?? '--'} KWH"),
          SfLinearGauge(
              markerPointers: [
                LinearShapePointer(
                  value: state.data.energy ?? 0,
                  shapeType: LinearShapePointerType.invertedTriangle,
                )
              ],
              axisTrackStyle: const LinearAxisTrackStyle(
                  gradient: LinearGradient(
                      colors: [Colors.purple, Colors.blue],
                      begin: Alignment.centerLeft,
                      end: Alignment.centerRight,
                      stops: [0.1, 0.5],
                      tileMode: TileMode.clamp))),
        ],
      );
    });
  }
}
