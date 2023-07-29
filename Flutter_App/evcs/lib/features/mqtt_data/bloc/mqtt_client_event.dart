part of 'mqtt_client_bloc.dart';

abstract class MqttClientEvent extends Equatable {
  const MqttClientEvent();

  @override
  List<Object> get props => [];
}

class ConnectToMqttEvent extends MqttClientEvent {
  final String topic;
  const ConnectToMqttEvent(this.topic);
}

class UpdateDataMqttEvent extends MqttClientEvent {
  final MqttReceivedData data;
  const UpdateDataMqttEvent(this.data);
}

class SubcribeTopicEvent extends MqttClientEvent {
  final String topic;
  const SubcribeTopicEvent(this.topic);
}

class UpdateSubcriptionEvent extends MqttClientEvent {
  final MqttSubcriptionStatus status;
  const UpdateSubcriptionEvent(this.status);
}

class UpdateConnectionStatusEvent extends MqttClientEvent {
  final MqttConnectionStatus status;
  const UpdateConnectionStatusEvent(this.status);
}

class ReconnectToMqttServerEvent extends MqttClientEvent {}

class DisconnectToMqttEvent extends MqttClientEvent {}
