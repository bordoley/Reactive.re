type context = {
  mutable connect: RxObservable.t(unit),
  timeoutSubscription: RxSerialDisposable.t,
};

let operator = {
  let subscribeToTimeout = ({connect, timeoutSubscription}) => {
    timeoutSubscription
    |> RxSerialDisposable.getInnerDisposable
    |> RxDisposable.dispose;
    timeoutSubscription
    |> RxSerialDisposable.setInnerDisposable(
         connect |> RxObservable.subscribe,
       );
  };

  let onNext = (ctx, delegate, next) => {
    delegate |> RxSubscriber.next(next);
    subscribeToTimeout(ctx);
  };

  let onComplete = ({timeoutSubscription}, delegate, exn) => {
    timeoutSubscription |> RxSerialDisposable.dispose;
    delegate |> RxSubscriber.complete(~exn?);
  };

  (~scheduler, due) => {
    let timeoutObservable =
      RaiseObservable.raise(RxTimeoutException.Exn)
      |> RxObservable.lift(DelayOperator.operator(~scheduler, due));

    subscriber => {
      let context = {
        connect: RxObservable.never,
        timeoutSubscription: RxSerialDisposable.create(),
      };

      let self =
        subscriber
        |> RxSubscriber.decorate1(~onNext, ~onComplete, context)
        |> RxSubscriber.addTeardown1(
             RxSerialDisposable.dispose,
             context.timeoutSubscription,
           );

      context.connect =
        timeoutObservable
        |> RxObservable.observe1(
             ~onNext=RxFunctions.alwaysUnit2,
             ~onComplete=RxSubscriber.forwardOnComplete,
             subscriber,
           );

      subscribeToTimeout(context);
      self;
    };
  };
};