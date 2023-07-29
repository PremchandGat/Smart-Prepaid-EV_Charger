import 'package:evcs/core/constants/language/english.dart';
import 'package:evcs/features/authentication/bloc/google_auth_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:google_sign_in/google_sign_in.dart';
import 'package:google_sign_in/widgets.dart';

class UserInfo extends StatelessWidget {
  const UserInfo({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<GoogleAuthBloc, GoogleAuthState>(
      builder: (context, state) {
        return state.currentUser == null
            ? const Text(Enlang.loginaAgain)
            : ListTile(
                leading: GoogleUserCircleAvatar(identity: state.currentUser!),
                title: Text(state.currentUser?.displayName ?? ""),
                subtitle: Text(state.currentUser!.email),
                trailing: IconButton(
                    onPressed: () =>
                        context.read<GoogleAuthBloc>().add(SignOutEvent()),
                    icon: Icon(Icons.logout)),
              );
      },
    );
  }
}
