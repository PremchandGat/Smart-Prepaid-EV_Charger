import 'package:evcs/features/authentication/UI/user_info.dart';
import 'package:evcs/features/payment_history/UI/widget/payment_tile.dart';
import 'package:flutter/material.dart';

class ProfileScreen extends StatelessWidget {
  const ProfileScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("User Profile"),
      ),
      body: Column(
        children: [
          const UserInfo(),
          Expanded(
              child: ListView(
            children: [
              for (int i = 0; i < 100; i++)
                PaymentTileWidget(
                  paymentInfo: i % 2 == 0
                      ? ReceivedAmount(
                          time: "time",
                          amount: i.toDouble(),
                          transactionID: "re")
                      : DeductedAmount(
                          amount: i.toDouble(),
                          consumedEnergy: i.toDouble(),
                          rate: i.toDouble(),
                          transactionID: "----------",
                          time: "time"),
                )
            ],
          ))
        ],
      ),
    );
  }
}
