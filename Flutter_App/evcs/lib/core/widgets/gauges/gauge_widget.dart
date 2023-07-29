
import 'package:evcs/core/constants/theme/graph_theme.dart';
import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class GaugeWidget extends StatefulWidget {
  const GaugeWidget({super.key});

  @override
  State<GaugeWidget> createState() => _GaugeWidgetState();
}

class _GaugeWidgetState extends State<GaugeWidget> {
  @override
  Widget build(BuildContext context) {
    return SfRadialGauge(enableLoadingAnimation: true, axes: <RadialAxis>[
      RadialAxis(
        axisLineStyle: GraphTheme.lineStyle,
        annotations: <GaugeAnnotation>[
          GraphTheme.gaugeAnnotation("text"),
        ],
        pointers: <GaugePointer>[GraphTheme.pointer(8)],
        showLastLabel: true,
        minimum: 0,
        maximum: 300,
        canRotateLabels: false,
        useRangeColorForAxis: false,
      )
    ]);
  }
}
