type t('a) =
  | Disposed
  | AutoDisposing(RxCompositeDisposable.t)
  | Decorating(
                t('b),
                RxAtomic.t(bool),
                (t('b), 'a) => unit,
                (t('b), option(exn)) => unit,
              ): t('a)
  | Decorating1(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 ('ctx0, t('b), 'a) => unit,
                 ('ctx0, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating2(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 'ctx1,
                 ('ctx0, 'ctx1, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating3(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating4(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating5(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) =>
                 unit,
               ): t('a)
  | Decorating6(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('b), 'a) => unit,
                 (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   t('b),
                   option(exn)
                 ) =>
                 unit,
               ): t('a)
  | Decorating7(
                 t('b),
                 RxAtomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
                 'ctx6,
                 (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   t('b),
                   'a
                 ) =>
                 unit,
                 (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   t('b),
                   option(exn)
                 ) =>
                 unit,
               ): t('a);

let rec asCompositeDisposable: type a. t(a) => RxCompositeDisposable.t =
  fun
  | Disposed => RxCompositeDisposable.disposed
  | AutoDisposing(disposable) => disposable
  | Decorating(delegate, _, _, _) => delegate |> asCompositeDisposable
  | Decorating1(delegate, _, _, _, _) => delegate |> asCompositeDisposable
  | Decorating2(delegate, _, _, _, _, _) => delegate |> asCompositeDisposable
  | Decorating3(delegate, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating4(delegate, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating5(delegate, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating6(delegate, _, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating7(delegate, _, _, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable;


let addTeardown = (teardown, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown(teardown)
  |> ignore;
  subscriber;
};

let addTeardown1 = (teardown, d0, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown1(teardown, d0)
  |> ignore;
  subscriber;
};

let addTeardown2 = (teardown, d0, d1, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown2(teardown, d0, d1)
  |> ignore;
  subscriber;
};

let addTeardown3 = (teardown, d0, d1, d2, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown3(teardown, d0, d1, d2)
  |> ignore;
  subscriber;
};

let addTeardown4 = (teardown, d0, d1, d2, d3, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown4(teardown, d0, d1, d2, d3)
  |> ignore;
  subscriber;
};

let addTeardown5 = (teardown, d0, d1, d2, d3, d4, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown5(teardown, d0, d1, d2, d3, d4)
  |> ignore;
  subscriber;
};

let addTeardown6 = (teardown, d0, d1, d2, d3, d4, d5, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown6(teardown, d0, d1, d2, d3, d4, d5)
  |> ignore;
  subscriber;
};

let addTeardown7 = (teardown, d0, d1, d2, d3, d4, d5, d6, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addTeardown7(teardown, d0, d1, d2, d3, d4, d5, d6)
  |> ignore;
  subscriber;
};

let asDisposable = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.asDisposable;

let create = () => {
  let disposable = RxCompositeDisposable.create();
  AutoDisposing(disposable);
};

let decorate = (~onNext, ~onComplete, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating(subscriber, isStopped, onNext, onComplete);
};

let decorate1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating1(subscriber, isStopped, ctx0, onNext, onComplete);
};

let decorate2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating2(subscriber, isStopped, ctx0, ctx1, onNext, onComplete);
};

let decorate3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating3(subscriber, isStopped, ctx0, ctx1, ctx2, onNext, onComplete);
};

let decorate4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating4(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    onNext,
    onComplete,
  );
};

let decorate5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating5(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    onNext,
    onComplete,
  );
};

let decorate6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, subscriber) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating6(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    onNext,
    onComplete,
  );
};

let decorate7 =
    (
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      subscriber,
    ) => {
  let isStopped = RxAtomic.make(false);
  subscriber |> addTeardown1(RxAtomic.setTrue, isStopped) |> ignore;
  Decorating7(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    onNext,
    onComplete,
  );
};

let dispose = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.dispose;

let disposed = Disposed;

let isDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.isDisposed;

let isStopped =
  fun
  | Disposed => true
  | AutoDisposing(disposable) => RxCompositeDisposable.isDisposed(disposable)
  | Decorating(_, isStopped, _, _)
  | Decorating1(_, isStopped, _, _, _)
  | Decorating2(_, isStopped, _, _, _, _)
  | Decorating3(_, isStopped, _, _, _, _, _)
  | Decorating4(_, isStopped, _, _, _, _, _, _)
  | Decorating5(_, isStopped, _, _, _, _, _, _, _)
  | Decorating6(_, isStopped, _, _, _, _, _, _, _, _)
  | Decorating7(_, isStopped, _, _, _, _, _, _, _, _, _) =>
    RxAtomic.get(isStopped);

let shouldComplete =
  fun
  | Disposed => false
  | AutoDisposing(disposable) =>
    ! RxCompositeDisposable.isDisposed(disposable)
  | Decorating(_, isStopped, _, _)
  | Decorating1(_, isStopped, _, _, _)
  | Decorating2(_, isStopped, _, _, _, _)
  | Decorating3(_, isStopped, _, _, _, _, _)
  | Decorating4(_, isStopped, _, _, _, _, _, _)
  | Decorating5(_, isStopped, _, _, _, _, _, _, _)
  | Decorating6(_, isStopped, _, _, _, _, _, _, _, _)
  | Decorating7(_, isStopped, _, _, _, _, _, _, _, _, _) =>
    ! RxAtomic.exchange(isStopped, true);


let completeWithResult = {
  let doComplete = (exn, subscriber) =>
    switch (subscriber) {
    | Disposed => ()
    | AutoDisposing(disposable) => disposable |> RxCompositeDisposable.dispose
    | Decorating(delegate, _, _, onComplete) => onComplete(delegate, exn)
    | Decorating1(delegate, _, ctx0, _, onComplete) =>
      onComplete(ctx0, delegate, exn)
    | Decorating2(delegate, _, ctx0, ctx1, _, onComplete) =>
      onComplete(ctx0, ctx1, delegate, exn)
    | Decorating3(delegate, _, ctx0, ctx1, ctx2, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, delegate, exn)
    | Decorating4(delegate, _, ctx0, ctx1, ctx2, ctx3, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, delegate, exn)
    | Decorating5(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, exn)
    | Decorating6(
        delegate,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        _,
        onComplete,
      ) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, delegate, exn)
    | Decorating7(
        delegate,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        _,
        onComplete,
      ) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, delegate, exn)
    };

  let completeDelegate = (exn, subscriber) =>
    switch (subscriber) {
    | Disposed => ()
    | AutoDisposing(_) => ()
    | Decorating(delegate, _, _, _) => delegate |> doComplete(exn)
    | Decorating1(delegate, _, _, _, _) => delegate |> doComplete(exn)
    | Decorating2(delegate, _, _, _, _, _) => delegate |> doComplete(exn)
    | Decorating3(delegate, _, _, _, _, _, _) => delegate |> doComplete(exn)
    | Decorating4(delegate, _, _, _, _, _, _, _) =>
      delegate |> doComplete(exn)
    | Decorating5(delegate, _, _, _, _, _, _, _, _) =>
      delegate |> doComplete(exn)
    | Decorating6(delegate, _, _, _, _, _, _, _, _, _) =>
      delegate |> doComplete(exn)
    | Decorating7(delegate, _, _, _, _, _, _, _, _, _, _) =>
      delegate |> doComplete(exn)
    };

  (~exn=?, subscriber) => {
    let shouldComplete = shouldComplete(subscriber);
    if (shouldComplete) {
      try (doComplete(exn, subscriber)) {
      | exn => subscriber |> completeDelegate(Some(exn))
      };
    };
    shouldComplete;
  };
};

let complete = (~exn=?, subscriber) =>
  subscriber |> completeWithResult(~exn?) |> ignore;

let next = {
  let doNext = (next, subscriber) =>
    switch (subscriber) {
    | Disposed => ()
    | AutoDisposing(_) => ()
    | Decorating(delegate, _, onNext, _) => onNext(delegate, next)
    | Decorating1(delegate, _, ctx0, onNext, _) =>
      onNext(ctx0, delegate, next)
    | Decorating2(delegate, _, ctx0, ctx1, onNext, _) =>
      onNext(ctx0, ctx1, delegate, next)
    | Decorating3(delegate, _, ctx0, ctx1, ctx2, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, delegate, next)
    | Decorating4(delegate, _, ctx0, ctx1, ctx2, ctx3, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, delegate, next)
    | Decorating5(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, next)
    | Decorating6(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, delegate, next)
    | Decorating7(
        delegate,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        onNext,
        _,
      ) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, delegate, next)
    };

  (next, subscriber) => {
    let isStopped = subscriber |> isStopped;
    if (! isStopped) {
      try (doNext(next, subscriber)) {
      | exn => subscriber |> complete(~exn)
      };
    };
  };
};

let forwardOnComplete = (subscriber, exn) => subscriber |> complete(~exn?);

let forwardOnComplete1 = (_, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete2 = (_, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete3 = (_, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete4 = (_, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete5 = (_, _, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete6 = (_, _, _, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete7 = (_, _, _, _, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnNext = (subscriber, v) => subscriber |> next(v);

let forwardOnNext1 = (_, subscriber, v) => subscriber |> next(v);

let forwardOnNext2 = (_, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext3 = (_, _, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext4 = (_, _, _, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext5 = (_, _, _, _, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext6 = (_, _, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let forwardOnNext7 = (_, _, _, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let notify = (notif, subscriber) =>
  switch (notif) {
  | RxNotification.Next(v) => subscriber |> next(v)
  | RxNotification.Complete(exn) => subscriber |> complete(~exn?)
  };

let raiseIfDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.raiseIfDisposed;