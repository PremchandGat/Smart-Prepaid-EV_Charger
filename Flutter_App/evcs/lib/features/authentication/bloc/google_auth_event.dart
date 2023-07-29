part of 'google_auth_bloc.dart';

abstract class GoogleAuthEvent extends Equatable {
  const GoogleAuthEvent();

  @override
  List<Object> get props => [];
}

class SignInWithGoogleEvent extends GoogleAuthEvent {}

class UpdateCurrentUserStatusEvent extends GoogleAuthEvent {
  final GoogleSignInAccount? user;
  const UpdateCurrentUserStatusEvent(this.user);
}

class SignOutEvent extends GoogleAuthEvent {}
