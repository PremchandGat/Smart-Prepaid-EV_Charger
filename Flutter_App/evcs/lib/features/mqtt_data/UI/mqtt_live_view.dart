import 'package:evcs/features/mqtt_data/UI/widgets/current_indicator.dart';
import 'package:evcs/features/mqtt_data/UI/widgets/energy_indicator.dart';
import 'package:evcs/features/mqtt_data/UI/widgets/mqtt_connection_status.dart';
import 'package:evcs/features/mqtt_data/UI/widgets/power_indicator.dart';
import 'package:evcs/features/mqtt_data/UI/widgets/voltage_indicator.dart';
import 'package:evcs/features/mqtt_data/bloc/mqtt_client_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class MqttLiveView extends StatelessWidget {
  const MqttLiveView({super.key, required this.topic});
  final String topic;

  @override
  Widget build(BuildContext context) {
    return BlocProvider(
      create: (context) => MqttClientBloc()..add(ConnectToMqttEvent(topic)),
      child: BlocBuilder<MqttClientBloc, MqttClientState>(
          builder: (context, state) {
        return Column(
          children: [
            MqttConnectionStatusWidget(status: state.status),
            Text(
              "Connected to \"${state.topic}\"",
              style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            SizedBox(
              height: MediaQuery.of(context).size.width * 1.2,
              child: const Stack(
                alignment: AlignmentDirectional.bottomCenter,
                children: [
                  Positioned(top: 0, child: PowerIndicatorWidget()),
                  Row(
                    children: [
                      CurrentIndicatorWidget(),
                      VoltageIndicatorWidget()
                    ],
                  ),
                ],
              ),
            ),
            const EnergyIndicatorWidget(),
          ],
        );
      }),
    );
  }
}
