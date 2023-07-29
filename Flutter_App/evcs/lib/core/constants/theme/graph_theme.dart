import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class GraphTheme {
  static GaugeAnnotation gaugeAnnotation(String text, {double fontSize = 16}) =>
      GaugeAnnotation(
          axisValue: 50,
          widget: Text(text,
              style:
                  TextStyle(fontSize: fontSize, fontWeight: FontWeight.bold)),
          angle: 90,
          positionFactor: 0.8);

  static const AxisLineStyle lineStyle = AxisLineStyle(
      thickness: 15,
      cornerStyle: CornerStyle.bothFlat,
      gradient: SweepGradient(
          colors: [Colors.yellow, Colors.green, Colors.red],
          stops: <double>[0, 0.33, 0.75]));

  static NeedlePointer pointer(double value) => NeedlePointer(
        value: value,
        needleStartWidth: 0.2,
        needleEndWidth: 8,
        needleLength: 0.7,
        enableDragging: true,
        enableAnimation: true,
        tailStyle: const TailStyle(width: 8, length: 0.15),
        animationType: AnimationType.bounceOut,
      );
}
