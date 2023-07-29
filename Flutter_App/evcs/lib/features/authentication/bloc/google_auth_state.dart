part of 'google_auth_bloc.dart';

enum AuthStatus { unknown, signingIn, signedIn, signingOut, signedOut, failed }

GoogleSignIn _googleSignIn = GoogleSignIn(
  scopes: [
    'email',
    'https://www.googleapis.com/auth/userinfo.profile',
  ],
);

class GoogleAuthState extends Equatable {
  final AuthStatus status;
  final String googleAuthKey;
  final GoogleSignInAccount? currentUser;

  const GoogleAuthState(
      {this.status = AuthStatus.unknown,
      this.googleAuthKey = "",
      this.currentUser});

  GoogleAuthState updateCurrentUser(GoogleSignInAccount? currentUser) {
    return GoogleAuthState(
        currentUser: currentUser, status: status, googleAuthKey: googleAuthKey);
  }

  GoogleAuthState copyWith({
    AuthStatus? status,
    String? googleAuthKey,
  }) {
    return GoogleAuthState(
        currentUser: currentUser,
        status: status ?? this.status,
        googleAuthKey: googleAuthKey ?? this.googleAuthKey);
  }

  @override
  List<Object?> get props => [status, googleAuthKey, currentUser];
}

class GoogleAuthInitial extends GoogleAuthState {}
