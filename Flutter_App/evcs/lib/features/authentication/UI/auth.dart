import 'package:evcs/core/constants/images/images.dart';
import 'package:evcs/core/constants/language/english.dart';
import 'package:evcs/features/authentication/bloc/google_auth_bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class GoogleAuth extends StatelessWidget {
  const GoogleAuth({super.key});
  String _getAuthStatusMessage(AuthStatus status) {
    switch (status) {
      case AuthStatus.signingIn:
        return Enlang.signingIn;
      case AuthStatus.signedIn:
        return Enlang.alreadySignedIn;
      case AuthStatus.failed:
        return Enlang.faiedTryAgain;
      default:
        return Enlang.signInWithGoogle;
    }
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<GoogleAuthBloc, GoogleAuthState>(
      buildWhen: (prev, curr) => prev.status != curr.status,
      builder: (context, state) {
        return FloatingActionButton.extended(
          onPressed: () =>
              context.read<GoogleAuthBloc>().add(SignInWithGoogleEvent()),
          label: Text(_getAuthStatusMessage(state.status)),
          icon: state.status == AuthStatus.signingIn
              ? const CircularProgressIndicator()
              : SizedBox(height: 56, child: Image.asset(MyImages.google)),
        );
      },
    );
  }
}
