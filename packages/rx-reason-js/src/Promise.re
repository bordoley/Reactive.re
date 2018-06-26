[@bs.send]
external promiseThen :
  (Js.Promise.t('a), 'a => unit, Js.Promise.error => unit) =>
  Js.Promise.t('a) =
  "then";

let toSingle = {
  let toSingleSubscribe = (promise, singleSubscriber) => {
    let onSuccessIfNotDisposed = result =>
      if (! RxReason.SingleSubscriber.isDisposed(singleSubscriber)) {
        singleSubscriber |> RxReason.SingleSubscriber.onSuccess(result);
      };

    let onErrorIfNotDisposed = err =>
      if (! RxReason.SingleSubscriber.isDisposed(singleSubscriber)) {
        singleSubscriber
        |> RxReason.SingleSubscriber.onError(PromiseException.Exn(err));
      };

    promise
    |. promiseThen(onSuccessIfNotDisposed, onErrorIfNotDisposed)
    |> ignore;
  };

  promise => RxReason.Single.create1(toSingleSubscribe, promise);
};

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