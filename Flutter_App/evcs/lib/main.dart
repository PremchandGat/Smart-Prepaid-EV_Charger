import 'package:evcs/core/constants/language/english.dart';
import 'package:evcs/features/authentication/bloc/google_auth_bloc.dart';
import 'package:evcs/features/scanner/bloc/scanner_bloc.dart';
import 'package:evcs/screens/auth/auth_screen.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'screens/home/home_screen.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MultiBlocProvider(
        providers: [
          BlocProvider<ScannerBloc>(
            create: (BuildContext context) => ScannerBloc(),
          ),
          BlocProvider<GoogleAuthBloc>(
            create: (BuildContext context) =>
                GoogleAuthBloc()..add(SignInWithGoogleEvent()),
          ),
        ],
        child: MaterialApp(
          title: Enlang.evcs,
          theme: ThemeData(
            colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
            useMaterial3: true,
          ),
          home: BlocBuilder<GoogleAuthBloc, GoogleAuthState>(
            builder: (context, state) {
              return state.currentUser == null
                  ? const AuthWithGoogleScreen()
                  : const HomePage();
            },
          ),
        ));
  }
}
