import 'dart:convert';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:evcs/core/constants/apis/apis.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

part 'mqtt_client_event.dart';
part 'mqtt_client_state.dart';

class MqttClientBloc extends Bloc<MqttClientEvent, MqttClientState> {
  MqttClientBloc()
      : super(MqttClientState(
            client:
                MqttServerClient.withPort(MyApis.ipAddress, 'ffffl', 1883))) {
    on<ConnectToMqttEvent>(_connectToMqtt);
    on<UpdateDataMqttEvent>(_updateData);
    on<SubcribeTopicEvent>(_subcribeTopic);
    on<UpdateConnectionStatusEvent>(_updateConnectionStatus);
    on<UpdateSubcriptionEvent>(_updateSubscribeStatus);
    on<DisconnectToMqttEvent>(_disconnectToMqtt);
    on<ReconnectToMqttServerEvent>(_retryConnection);
  }

  @override
  Future<void> close() {
    state.client.disconnect();
    return super.close();
  }

  void _retryConnection(
      ReconnectToMqttServerEvent event, Emitter<MqttClientState> emit) {
    add(ConnectToMqttEvent(state.topic));
  }

  void _disconnectToMqtt(
      DisconnectToMqttEvent event, Emitter<MqttClientState> emit) {
    state.client.disconnect();
  }

  // unconnected
  void _updateConnectionStatus(
      UpdateConnectionStatusEvent event, Emitter<MqttClientState> emit) {
    print("Connected to................");
    if (event.status == MqttConnectionStatus.connected) {
      print("Connected to...////////.");
      add(SubcribeTopicEvent(state.topic));
    }
    emit(state.copyData(status: event.status));
  }

// subscribe to topic succeeded
  void _updateSubscribeStatus(
      UpdateSubcriptionEvent event, Emitter<MqttClientState> emit) async {
    emit(state.copyData(subStatus: event.status));
  }

  _subcribeTopic(SubcribeTopicEvent event, Emitter<MqttClientState> emit) {
    emit(state.copyData(topic: event.topic));
    print(state.topic);
    state.client.subscribe(state.topic, MqttQos.atLeastOnce);
  }

  _updateData(UpdateDataMqttEvent event, Emitter<MqttClientState> emit) async {
    print("Adding data to :${event.data}");
    emit(state.copyData(data: event.data));
  }

  _connectToMqtt(
      ConnectToMqttEvent event, Emitter<MqttClientState> emit) async {
    state.client.logging(on: true);
    add(const UpdateConnectionStatusEvent(MqttConnectionStatus.connecting));
    state.client.onConnected = () =>
        add(const UpdateConnectionStatusEvent(MqttConnectionStatus.connected));
    state.client.onDisconnected = () => add(
        const UpdateConnectionStatusEvent(MqttConnectionStatus.disconnected));
    state.client.onUnsubscribed = (String? a) =>
        add(const UpdateSubcriptionEvent(MqttSubcriptionStatus.unsubscribed));
    state.client.onSubscribed = (String? a) =>
        add(const UpdateSubcriptionEvent(MqttSubcriptionStatus.subcribed));
    state.client.onSubscribeFail = (String? a) =>
        add(const UpdateSubcriptionEvent(MqttSubcriptionStatus.failed));

    final connMessage = MqttConnectMessage()
        .authenticateAs('username', 'password')
        .withWillTopic('willtopic')
        .withWillMessage('Will message')
        .startClean()
        .withWillQos(MqttQos.atLeastOnce);
    state.client.connectionMessage = connMessage;
    try {
      emit(state.copyData(topic: event.topic));
      state.client.keepAlivePeriod = 20;
      await state.client
          .connect()
          .then((value) => add(SubcribeTopicEvent(event.topic)));
      // add(SubcribeTopicEvent(event.topic));
      print("Subscribning to");
    } catch (e) {
      print('Exception: $e');
      state.client.disconnect();
    }

    state.client.updates?.listen((List<MqttReceivedMessage<MqttMessage>> c) {
      final MqttPublishMessage message = c[0].payload as MqttPublishMessage;
      final pt =
          MqttPublishPayload.bytesToStringAsString(message.payload.message);
      //  final recMess = c[0].payload as MqttPublishMessage;
      var data = jsonDecode(pt);

      add(UpdateDataMqttEvent(MqttReceivedData(
          volt: data['v'].runtimeType == String
              ? double.parse(data['v'])
              : (data['v'].runtimeType == int
                  ? data['v'].toDouble()
                  : data['v']),
          current: data['i'].runtimeType == String
              ? double.parse(data['i'])
              : (data['i'].runtimeType == int
                  ? data['i'].toDouble()
                  : data['i']),
          power: data['p'].runtimeType == String
              ? double.parse(data['p'])
              : (data['p'].runtimeType == int
                  ? data['p'].toDouble()
                  : data['p']),
          temprature: data['t'].runtimeType == String
              ? double.parse(data['t'])
              : (data['t'].runtimeType == int
                  ? data['t'].toDouble()
                  : data['t']),
          energy: data['e'].runtimeType == String
              ? double.parse(data['e'])
              : (data['e'].runtimeType == int
                  ? data['e'].toDouble()
                  : data['e']))));
      print(
          'EXAMPLE::Change notification:: topic is <${c[0].topic}>, payload is <-- $pt -->');
    });
  }
}
