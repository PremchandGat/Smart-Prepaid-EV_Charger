import 'package:evcs/core/constants/language/english.dart';
import 'package:evcs/features/authentication/UI/auth.dart';
import 'package:flutter/material.dart';

class AuthWithGoogleScreen extends StatelessWidget {
  const AuthWithGoogleScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return const Scaffold(
      body: Column(
        mainAxisAlignment: MainAxisAlignment.end,
        children: [
          SizedBox(
            height: 100,
          ),
          Expanded(
            child: Text(
              Enlang.welcome,
              style: TextStyle(fontSize: 32, fontWeight: FontWeight.bold),
            ),
          ),
          //SizedBox.expand(),
          Align(child: GoogleAuth()),
          SizedBox(
            height: 100,
          ),
          Text(
            Enlang.appInfo,
            textAlign: TextAlign.center,
            style: TextStyle(fontWeight: FontWeight.w500),
          )
        ],
      ),
    );
  }
}
