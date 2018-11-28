[@bs.send]
external promiseThen :
  (Js.Promise.t('a), 'a => unit, Js.Promise.error => unit) =>
  Js.Promise.t('a) =
  "then";

let toObservable = {
  let toObservableSubscribe = (promise, subscriber) => {
    let onSuccessIfNotDisposed = result =>
      if (! RxSubscriber.isDisposed(subscriber)) {
        subscriber |> RxSubscriber.next(result);
        subscriber |> RxSubscriber.complete;
      };

    let onErrorIfNotDisposed = err =>
      if (! RxSubscriber.isDisposed(subscriber)) {
        subscriber |> RxSubscriber.complete(~exn=RxPromiseException.Exn(err));
      };

    promise
    |. promiseThen(onSuccessIfNotDisposed, onErrorIfNotDisposed)
    |> ignore;
  };

  promise => RxObservable.create1(toObservableSubscribe, promise);
};

let fromObservable = {
  let onNext = (last, _, _, next) => RxMutableOption.set(next, last);

  let onComplete = (last, resolve, reject, exn) =>
    switch (exn) {
    | Some(exn) => reject(. exn)
    | None when ! RxMutableOption.isEmpty(last) =>
      let lastValue = RxMutableOption.get(last);
      resolve(. lastValue);
    | _ => ()
    };

  observable =>
    Js.Promise.make((~resolve, ~reject) => {
      let last = RxMutableOption.create();

      observable
      |> RxObservable.observe3(~onNext, ~onComplete, last, resolve, reject)
      |> RxObservable.subscribe
      |> ignore;
    });
};