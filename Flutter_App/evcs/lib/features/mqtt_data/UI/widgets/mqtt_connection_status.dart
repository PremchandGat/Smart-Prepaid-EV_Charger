import 'package:evcs/features/mqtt_data/bloc/mqtt_client_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class MqttConnectionStatusWidget extends StatelessWidget {
  const MqttConnectionStatusWidget({super.key, required this.status});
  final MqttConnectionStatus status;

  Widget _getWidget(context) {
    String message = "";
    Color color = Colors.black;
    Widget? reconnectButton;
    switch (status) {
      case MqttConnectionStatus.connected:
        {
          message = "Connected";
          color = Colors.green;
          break;
        }
      case MqttConnectionStatus.connecting:
        {
          message = "Connecting...";
          color = Colors.yellow;
          break;
        }
      case MqttConnectionStatus.disconnected:
        {
          message = "Disconnected";
          color = Colors.red;
          reconnectButton = IconButton(
              onPressed: () => BlocProvider.of<MqttClientBloc>(context)
                  .add(ReconnectToMqttServerEvent()),
              icon: const Icon(Icons.refresh));
          break;
        }
      default:
        message = "Unknown";
        color = Colors.blue;
        break;
    }
    return Container(
      decoration: BoxDecoration(color: color),
      child: Column(
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Padding(
                padding: const EdgeInsets.all(2.0),
                child: Text(message),
              ),
              reconnectButton ?? const SizedBox()
            ],
          ),
          status == MqttConnectionStatus.connecting
              ? const LinearProgressIndicator()
              : const SizedBox()
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return _getWidget(context);
  }
}
