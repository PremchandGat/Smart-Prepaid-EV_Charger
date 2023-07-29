import 'package:evcs/core/constants/theme/graph_theme.dart';
import 'package:evcs/features/mqtt_data/bloc/mqtt_client_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class PowerIndicatorWidget extends StatelessWidget {
  const PowerIndicatorWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MqttClientBloc, MqttClientState>(
        buildWhen: (previous, current) =>
            (previous.data.power != current.data.power ||
                current.data.temprature != previous.data.temprature),
        builder: (context, state) {
          return SizedBox(
            width: MediaQuery.of(context).size.width * 0.8,
            height: MediaQuery.of(context).size.width * 0.8,
            child:
                SfRadialGauge(enableLoadingAnimation: true, axes: <RadialAxis>[
              RadialAxis(
                annotations: <GaugeAnnotation>[
                  GraphTheme.gaugeAnnotation(
                      "${state.data.temprature ?? '--'}°C",
                      fontSize: 12),
                ],
                axisLineStyle: const AxisLineStyle(
                    thickness: 3,
                    gradient: SweepGradient(
                        colors: [Colors.green, Colors.orange, Colors.red],
                        stops: <double>[0, 0.4, 0.7])),
                pointers: <GaugePointer>[
                  NeedlePointer(
                    value: state.data.temprature ?? 0,
                    needleStartWidth: 0.2,
                    needleEndWidth: 3,
                  )
                ],
                centerY: 0.6,
                centerX: 0.35,
                radiusFactor: 0.3,
                interval: 20,
              ),
              RadialAxis(
                axisLineStyle: GraphTheme.lineStyle,
                annotations: <GaugeAnnotation>[
                  GraphTheme.gaugeAnnotation(
                      "${state.data.power ?? '--'}Watts"),
                ],
                pointers: <GaugePointer>[
                  GraphTheme.pointer(state.data.power ?? 0)
                ],
                showLastLabel: true,
                minimum: 0,
                canRotateLabels: false,
                useRangeColorForAxis: false,
                canScaleToFit: true,
              )
            ]),
          );
        });
  }
}
