[@bs.send]
external promiseThen :
  (Js.Promise.t('a), 'a => unit, Js.Promise.error => unit) =>
  Js.Promise.t('a) =
  "then";

let toSingle = promise =>
  RxReason.Single.create((~onSuccess, ~onError, ~cancellationToken) => {
    let onSuccessIfNotDisposed = next =>
      if (! RxReason.CompositeDisposable.isDisposed(cancellationToken)) {
        onSuccess(next);
      };

    let onErrorIfNotDisposed = err =>
      if (! RxReason.CompositeDisposable.isDisposed(cancellationToken)) {
        onError(PromiseException.Exn(err));
      };

    promise
    |. promiseThen(onSuccessIfNotDisposed, onErrorIfNotDisposed)
    |> ignore;
  });

let fromSingle = {
  let onSuccess = (resolve, _, a) => resolve(. a);
  let onError = (_, reject, exn) => reject(. exn);

  single =>
    Js.Promise.make((~resolve, ~reject) =>
      single
      |> RxReason.Single.subscribeWith2(
           ~onSuccess,
           ~onError,
           resolve,
           reject,
         )
      |> ignore
    );
};