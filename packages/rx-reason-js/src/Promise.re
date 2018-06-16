[@bs.send] external promiseThen : (Js.Promise.t('a), 'a => unit, Js.Promise.error => unit) => Js.Promise.t('a) = "then";

exception JSPromiseErrorException(Js.Promise.error);

let toSingle = (
  promise: Js.Promise.t('a),
): RxReason.Single.t('a) => RxReason.Single.create(
  (~onSuccess, ~onError) =>{
    let disposable = RxReason.Disposable.empty();

    let onSuccessIfNotDisposed = next => {
      if(!RxReason.Disposable.isDisposed(disposable)) {
        onSuccess(next);
      };
    };

    let onErrorIfNotDisposed = err => {
      if(!RxReason.Disposable.isDisposed(disposable)) {
        onError(JSPromiseErrorException(err));
      };
    };

    promise |. promiseThen(onSuccessIfNotDisposed, onErrorIfNotDisposed) |> ignore;
    disposable;
  },
);

let fromSingle = (
  single: RxReason.Single.t('a),
): Js.Promise.t('a) => Js.Promise.make((~resolve, ~reject) => {
  single |> RxReason.Single.subscribeWith(
    ~onSuccess=a => resolve(. a), 
    ~onError=e => reject(. e),
  ) |> ignore;
});