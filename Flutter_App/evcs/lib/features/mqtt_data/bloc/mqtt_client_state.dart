part of 'mqtt_client_bloc.dart';

enum MqttConnectionStatus { unknown, connecting, connected, disconnected }

enum MqttSubcriptionStatus {
  unknown,
  subcribing,
  subcribed,
  unsbuscribing,
  unsubscribed,
  failed
}

class MqttReceivedData {
  final double? volt;
  final double? current;
  final double? power;
  final double? temprature;
  final double? energy;
  const MqttReceivedData(
      {this.current, this.power, this.volt, this.temprature, this.energy});
}

class MqttClientState extends Equatable {
  final String topic;
  final MqttConnectionStatus status;
  final MqttSubcriptionStatus subStatus;
  final MqttServerClient client;

  final MqttReceivedData data;

  const MqttClientState(
      {this.topic = "",
      required this.client,
      this.status = MqttConnectionStatus.unknown,
      this.subStatus = MqttSubcriptionStatus.unknown,
      this.data = const MqttReceivedData()});

  MqttClientState copyData({
    MqttReceivedData? data,
    MqttConnectionStatus? status,
    MqttSubcriptionStatus? subStatus,
    String? topic,
  }) {
    return MqttClientState(
        client: client,
        subStatus: subStatus ?? this.subStatus,
        data: data ?? this.data,
        status: status ?? this.status,
        topic: topic ?? this.topic);
  }

  @override
  List<Object?> get props => [topic, status, client, data];
}
