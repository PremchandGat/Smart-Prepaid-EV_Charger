import 'package:evcs/core/constants/theme/graph_theme.dart';
import 'package:evcs/features/mqtt_data/bloc/mqtt_client_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class VoltageIndicatorWidget extends StatelessWidget {
  const VoltageIndicatorWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MqttClientBloc, MqttClientState>(
        buildWhen: (previous, current) =>
            previous.data.volt != current.data.volt,
        builder: (context, state) {
          return SizedBox(
            width: MediaQuery.of(context).size.width * 0.5,
            height: MediaQuery.of(context).size.width * 0.5,
            child:
                SfRadialGauge(enableLoadingAnimation: true, axes: <RadialAxis>[
              RadialAxis(
                axisLineStyle: GraphTheme.lineStyle,
                annotations: <GaugeAnnotation>[
                  GraphTheme.gaugeAnnotation("${state.data.volt ?? '--'}Volts"),
                ],
                pointers: <GaugePointer>[
                  GraphTheme.pointer(state.data.volt ?? 0)
                ],
                showLastLabel: true,
                minimum: 0,
                maximum: 300,
                canRotateLabels: false,
                canScaleToFit: true,
                useRangeColorForAxis: false,
              )
            ]),
          );
        });
  }
}
