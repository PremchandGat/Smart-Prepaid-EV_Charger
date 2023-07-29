import 'package:flutter/material.dart';

Future<bool> showAlertDialog(BuildContext context,
    {String title = "Alert!", String subtitle = "Are you sure?"}) async {
  bool? isAccepted = await showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        title: Text(title),
        content: Text(subtitle),
        actions: [
          TextButton(
              onPressed: () {
                return Navigator.pop(context, false);
              },
              child: const Text("Cancel")),
          ElevatedButton(
            child: const Text("OK"),
            onPressed: () {
              return Navigator.pop(context, true);
            },
          ),
        ],
      );
    },
  );
  return isAccepted ?? false;
}
