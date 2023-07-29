import 'package:evcs/core/constants/theme/graph_theme.dart';
import 'package:evcs/features/mqtt_data/bloc/mqtt_client_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class CurrentIndicatorWidget extends StatelessWidget {
  const CurrentIndicatorWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MqttClientBloc, MqttClientState>(
        buildWhen: (previous, current) =>
            previous.data.current != current.data.current,
        builder: (context, state) {
          return SizedBox(
            width: MediaQuery.of(context).size.width * 0.5,
            height: MediaQuery.of(context).size.width * 0.5,
            child:
                SfRadialGauge(enableLoadingAnimation: true, axes: <RadialAxis>[
              RadialAxis(
                axisLineStyle: GraphTheme.lineStyle,
                annotations: <GaugeAnnotation>[
                  GraphTheme.gaugeAnnotation(
                      "${state.data.current ?? '--'}Amps"),
                ],
                pointers: <GaugePointer>[
                  GraphTheme.pointer(state.data.current ?? 0)
                ],
                showLastLabel: true,
                minimum: 0,
                maximum: 10,
                canScaleToFit: true,
                canRotateLabels: false,
                useRangeColorForAxis: false,
              )
            ]),
          );
        });
  }
}
