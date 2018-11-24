let merge = {
  let onNext = (_, lock, subscriber, next) => {
    Lock.acquire(lock);
    subscriber |> Subscriber.next(next);
    Lock.release(lock);
  };

  let onComplete = (activeCount, lock, subscriber, exn) => {
    Lock.acquire(lock);
    let shouldComplete =
      switch (exn) {
      | Some(_) => true
      | None =>
        let oldActiveCount = Atomic.decr(activeCount);
        oldActiveCount <= 0;
      };
    if (shouldComplete) {
      subscriber |> Subscriber.complete(~exn?);
    };
    Lock.release(lock);
  };

  let mergeSource = (count, observables, subscriber) => {
    let activeCount = Atomic.make(count);
    let lock = Lock.create();

    let rec loop =
      fun
      | [hd, ...tail] => {
          let innerSubscriber =
            Subscriber.create3(
              ~onNext,
              ~onComplete,
              activeCount,
              lock,
              subscriber,
            );

          hd |> Observable.subscribeWith(innerSubscriber);

          subscriber
          |> Subscriber.addTeardown1(
               Disposable.dispose,
               innerSubscriber |> Subscriber.asDisposable,
             )
          |> ignore;
          loop(tail);
        }
      | [] => ();

    loop(observables);
  };

  observables => {
    let count = observables |> Lists.length;

    Observable.create2(mergeSource, count, observables);
  };
};