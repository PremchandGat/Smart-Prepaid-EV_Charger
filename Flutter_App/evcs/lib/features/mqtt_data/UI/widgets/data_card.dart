import 'package:flutter/material.dart';

class DataCard extends StatelessWidget {
  const DataCard({super.key, required this.data, required this.icon});
  final String data;
  final IconData icon;

  @override
  Widget build(BuildContext context) {
    return Container(
      width: MediaQuery.of(context).size.width * 0.5,
      padding: const EdgeInsets.all(10),
      child: Card(
        child: FittedBox(
          child: Padding(
            padding: const EdgeInsets.all(10.0),
            child: Row(
              children: [
                Icon(
                  icon,
                  size: 32,
                ),
                Text(
                  data,
                  style: const TextStyle(fontSize: 32),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
