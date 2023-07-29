import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:google_sign_in/google_sign_in.dart';

part 'google_auth_event.dart';
part 'google_auth_state.dart';

class GoogleAuthBloc extends Bloc<GoogleAuthEvent, GoogleAuthState> {
  GoogleAuthBloc() : super(GoogleAuthInitial()) {
    on<SignInWithGoogleEvent>(_handleSignIn);
    on<UpdateCurrentUserStatusEvent>(_updateUser);
    on<SignOutEvent>(_signOutUser);
  }

  _signOutUser(SignOutEvent event, Emitter<GoogleAuthState> emit) {
    _googleSignIn.signOut();
  }

  _updateUser(
      UpdateCurrentUserStatusEvent event, Emitter<GoogleAuthState> emit) async {
    if (event.user == null) {
      emit(state.copyWith(status: AuthStatus.signedOut));
    }
    emit(state.updateCurrentUser(event.user));
  }

  Future<void> _handleSignIn(
      SignInWithGoogleEvent event, Emitter<GoogleAuthState> emit) async {
    try {
      _googleSignIn.onCurrentUserChanged.listen((GoogleSignInAccount? account) {
        add(UpdateCurrentUserStatusEvent(account));
      });
      emit(state.copyWith(status: AuthStatus.signingIn));
      await _googleSignIn.signIn().then((result) async {
        await result?.authentication.then(
            (key) => emit(state.copyWith(
                status: AuthStatus.signedIn, googleAuthKey: key.accessToken)),
            onError: (err) => emit(
                state.copyWith(status: AuthStatus.failed, googleAuthKey: "")));
      });
    } catch (error) {
      emit(state.copyWith(status: AuthStatus.failed, googleAuthKey: ""));
    }
  }
}
