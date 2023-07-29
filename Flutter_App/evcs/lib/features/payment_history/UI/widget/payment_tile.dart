import 'package:flutter/material.dart';

abstract class PaymentInfo {
  final String transactionID;
  final double amount;
  final String time;
  const PaymentInfo(
      {required this.amount, required this.time, required this.transactionID});
}

class ReceivedAmount extends PaymentInfo {
  const ReceivedAmount({
    required super.time,
    required super.amount,
    required super.transactionID,
  });
}

class DeductedAmount extends PaymentInfo {
  final double rate;
  final double consumedEnergy;
  DeductedAmount({
    required super.amount,
    required this.consumedEnergy,
    required this.rate,
    required super.transactionID,
    required super.time,
  });
}

class PaymentTileWidget extends StatelessWidget {
  const PaymentTileWidget({super.key, required this.paymentInfo});
  final PaymentInfo paymentInfo;
  @override
  Widget build(BuildContext context) {
    return Container(
      margin: const EdgeInsets.all(2),
      color: paymentInfo is ReceivedAmount ? Colors.green[50] : Colors.red[50],
      child: paymentInfo is ReceivedAmount
          ? ListTile(
              leading: const Icon(Icons.call_received),
              title: Text("Transaction Id ${paymentInfo.transactionID}"),
              trailing: Text("RS. ${paymentInfo.amount}"),
              subtitle: Text("Time: ${paymentInfo.time}"),
            )
          : ListTile(
              leading: const Icon(Icons.call_made),
              title: Text("Transaction Id ${paymentInfo.transactionID}"),
              trailing: Text("RS. ${paymentInfo.amount}"),
              subtitle: Text(
                  "Energy: ${(paymentInfo as DeductedAmount).consumedEnergy}KWH, Rate: ${(paymentInfo as DeductedAmount).rate}/Unit\nTime: ${paymentInfo.time}"),
            ),
    );
  }
}
