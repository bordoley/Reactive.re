type source('a) = RxSubscriber.t('a) => unit;
type source1('ctx0, 'a) = ('ctx0, RxSubscriber.t('a)) => unit;
type source2('ctx0, 'ctx1, 'a) = ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit;
type source3('ctx0, 'ctx1, 'ctx2, 'a) =
  ('ctx0, 'ctx1, 'ctx2, RxSubscriber.t('a)) => unit;
type source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, RxSubscriber.t('a)) => unit;
type source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, RxSubscriber.t('a)) => unit;
type source6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, RxSubscriber.t('a)) => unit;
type source7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, RxSubscriber.t('a)) =>
  unit;

type t('a) =
  | Source(source('b), RxOperator.t('b, 'a)): t('a)
  | Source1(source1('ctx0, 'b), 'ctx0, RxOperator.t('b, 'a)): t('a)
  | Source2(source2('ctx0, 'ctx1, 'b), 'ctx0, 'ctx1, RxOperator.t('b, 'a)): t(
                                                                    'a,
                                                                    )
  | Source3(
             source3('ctx0, 'ctx1, 'ctx2, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source4(
             source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source5(
             source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source6(
             source6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             'ctx5,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source7(
             source7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             'ctx5,
             'ctx6,
             RxOperator.t('b, 'a),
           ): t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

let create = onSubscribe => Source(onSubscribe, RxFunctions.identity);
let create1 = (onSubscribe, ctx0) =>
  Source1(onSubscribe, ctx0, RxFunctions.identity);
let create2 = (onSubscribe, ctx0, ctx1) =>
  Source2(onSubscribe, ctx0, ctx1, RxFunctions.identity);
let create3 = (onSubscribe, ctx0, ctx1, ctx2) =>
  Source3(onSubscribe, ctx0, ctx1, ctx2, RxFunctions.identity);
let create4 = (onSubscribe, ctx0, ctx1, ctx2, ctx3) =>
  Source4(onSubscribe, ctx0, ctx1, ctx2, ctx3, RxFunctions.identity);
let create5 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  Source5(onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, RxFunctions.identity);
let create6 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
  Source6(
    onSubscribe,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    RxFunctions.identity,
  );
let create7 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
  Source7(
    onSubscribe,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    RxFunctions.identity,
  );

let liftOperator = (op0, op1, subscriber) => op0 @@ op1 @@ subscriber;

let lift = (operator, observable) =>
  switch (observable) {
  | Source(source, op) => Source(source, liftOperator(op, operator))
  | Source1(source, ctx0, op) =>
    Source1(source, ctx0, liftOperator(op, operator))
  | Source2(source, ctx0, ctx1, op) =>
    Source2(source, ctx0, ctx1, liftOperator(op, operator))
  | Source3(source, ctx0, ctx1, ctx2, op) =>
    Source3(source, ctx0, ctx1, ctx2, liftOperator(op, operator))
  | Source4(source, ctx0, ctx1, ctx2, ctx3, op) =>
    Source4(source, ctx0, ctx1, ctx2, ctx3, liftOperator(op, operator))
  | Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4, op) =>
    Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4, liftOperator(op, operator))
  | Source6(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, op) =>
    Source6(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      liftOperator(op, operator),
    )
  | Source7(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, op) =>
    Source7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      liftOperator(op, operator),
    )
  };

let never = Source(RxFunctions.alwaysUnit1, RxFunctions.identity);

module ObserveOperator = {
  type t('a) =
    | C0('a => unit, option(exn) => unit): t('a)
    | C1(('c0, 'a) => unit, ('c0, option(exn)) => unit, 'c0): t('a)
    | C2(('c0, 'c1, 'a) => unit, ('c0, 'c1, option(exn)) => unit, 'c0, 'c1): t(
                                                                    'a,
                                                                    )
    | C3(
          ('c0, 'c1, 'c2, 'a) => unit,
          ('c0, 'c1, 'c2, option(exn)) => unit,
          'c0,
          'c1,
          'c2,
        ): t('a)
    | C4(
          ('c0, 'c1, 'c2, 'c3, 'a) => unit,
          ('c0, 'c1, 'c2, 'c3, option(exn)) => unit,
          'c0,
          'c1,
          'c2,
          'c3,
        ): t('a)
    | C5(
          ('c0, 'c1, 'c2, 'c3, 'c4, 'a) => unit,
          ('c0, 'c1, 'c2, 'c3, 'c4, option(exn)) => unit,
          'c0,
          'c1,
          'c2,
          'c3,
          'c4,
        ): t('a)
    | C6(
          ('c0, 'c1, 'c2, 'c3, 'c4, 'c5, 'a) => unit,
          ('c0, 'c1, 'c2, 'c3, 'c4, 'c5, option(exn)) => unit,
          'c0,
          'c1,
          'c2,
          'c3,
          'c4,
          'c5,
        ): t('a)
    | C7(
          ('c0, 'c1, 'c2, 'c3, 'c4, 'c5, 'c6, 'a) => unit,
          ('c0, 'c1, 'c2, 'c3, 'c4, 'c5, 'c6, option(exn)) => unit,
          'c0,
          'c1,
          'c2,
          'c3,
          'c4,
          'c5,
          'c6,
        ): t('a);

  let onNext = (context: t('a), subscriber, next) =>
    switch (context) {
    | C0(onNext, _) =>
      onNext(next);
      subscriber |> RxSubscriber.next(next);
    | C1(onNext, _, c0) =>
      onNext(c0, next);
      subscriber |> RxSubscriber.next(next);
    | C2(onNext, _, c0, c1) =>
      onNext(c0, c1, next);
      subscriber |> RxSubscriber.next(next);
    | C3(onNext, _, c0, c1, c2) =>
      onNext(c0, c1, c2, next);
      subscriber |> RxSubscriber.next(next);
    | C4(onNext, _, c0, c1, c2, c3) =>
      onNext(c0, c1, c2, c3, next);
      subscriber |> RxSubscriber.next(next);
    | C5(onNext, _, c0, c1, c2, c3, c4) =>
      onNext(c0, c1, c2, c3, c4, next);
      subscriber |> RxSubscriber.next(next);
    | C6(onNext, _, c0, c1, c2, c3, c4, c5) =>
      onNext(c0, c1, c2, c3, c4, c5, next);
      subscriber |> RxSubscriber.next(next);
    | C7(onNext, _, c0, c1, c2, c3, c4, c5, c6) =>
      onNext(c0, c1, c2, c3, c4, c5, c6, next);
      subscriber |> RxSubscriber.next(next);
    };

  let onComplete = (context, subscriber, exn) =>
    switch (context) {
    | C0(_, onComplete) =>
      onComplete(exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C1(_, onComplete, c0) =>
      onComplete(c0, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C2(_, onComplete, c0, c1) =>
      onComplete(c0, c1, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C3(_, onComplete, c0, c1, c2) =>
      onComplete(c0, c1, c2, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C4(_, onComplete, c0, c1, c2, c3) =>
      onComplete(c0, c1, c2, c3, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C5(_, onComplete, c0, c1, c2, c3, c4) =>
      onComplete(c0, c1, c2, c3, c4, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C6(_, onComplete, c0, c1, c2, c3, c4, c5) =>
      onComplete(c0, c1, c2, c3, c4, c5, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    | C7(_, onComplete, c0, c1, c2, c3, c4, c5, c6) =>
      onComplete(c0, c1, c2, c3, c4, c5, c6, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    };

  let decorate = (context, subscriber) =>
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, context);

  let observe = (~onNext, ~onComplete, subscriber) => {
    let context = C0(onNext, onComplete);
    decorate(context, subscriber);
  };

  let observe1 = (~onNext, ~onComplete, ctx0, subscriber) => {
    let context = C1(onNext, onComplete, ctx0);
    decorate(context, subscriber);
  };

  let observe2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
    let context = C2(onNext, onComplete, ctx0, ctx1);
    decorate(context, subscriber);
  };

  let observe3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
    let context = C3(onNext, onComplete, ctx0, ctx1, ctx2);
    decorate(context, subscriber);
  };

  let observe4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, subscriber) => {
    let context = C4(onNext, onComplete, ctx0, ctx1, ctx2, ctx3);
    decorate(context, subscriber);
  };

  let observe5 =
      (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, subscriber) => {
    let context = C5(onNext, onComplete, ctx0, ctx1, ctx2, ctx3, ctx4);
    decorate(context, subscriber);
  };

  let observe6 =
      (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, subscriber) => {
    let context = C6(onNext, onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5);
    decorate(context, subscriber);
  };

  let observe7 =
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
    let context =
      C7(onNext, onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6);
    decorate(context, subscriber);
  };
};

let observe = (~onNext, ~onComplete, observable) =>
  observable |> lift(ObserveOperator.observe(~onNext, ~onComplete));

let observe1 = (~onNext, ~onComplete, ctx0, observable) =>
  observable |> lift(ObserveOperator.observe1(~onNext, ~onComplete, ctx0));

let observe2 = (~onNext, ~onComplete, ctx0, ctx1, observable) =>
  observable
  |> lift(ObserveOperator.observe2(~onNext, ~onComplete, ctx0, ctx1));

let observe3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) =>
  observable
  |> lift(ObserveOperator.observe3(~onNext, ~onComplete, ctx0, ctx1, ctx2));

let observe4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) =>
  observable
  |> lift(
       ObserveOperator.observe4(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3),
     );

let observe5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) =>
  observable
  |> lift(
       ObserveOperator.observe5(
         ~onNext,
         ~onComplete,
         ctx0,
         ctx1,
         ctx2,
         ctx3,
         ctx4,
       ),
     );

let observe6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, observable) =>
  observable
  |> lift(
       ObserveOperator.observe6(
         ~onNext,
         ~onComplete,
         ctx0,
         ctx1,
         ctx2,
         ctx3,
         ctx4,
         ctx5,
       ),
     );

let observe7 =
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
      observable,
    ) =>
  observable
  |> lift(
       ObserveOperator.observe7(
         ~onNext,
         ~onComplete,
         ctx0,
         ctx1,
         ctx2,
         ctx3,
         ctx4,
         ctx5,
         ctx6,
       ),
     );

let tryCompleteWithExceptionOrRaise = (exn, subscriber) => {
  let shouldRaise =
    subscriber |> RxSubscriber.completeWithResult(~exn) |> (!);
  if (shouldRaise) {
    /* This could happen when the onComplete is called synchronously in the
     * subscribe function which also throws.
     */
    raise(
      exn,
    );
  };
};

let subscribe = observable => {
  let subscriber = RxSubscriber.create();

  switch (observable) {
  | Source(source, op) =>
    let subscriber = op(subscriber);
    try (source(subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source1(source, ctx0, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source2(source, ctx0, ctx1, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source3(source, ctx0, ctx1, ctx2, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source4(source, ctx0, ctx1, ctx2, ctx3, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source6(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source7(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  };

  subscriber |> RxSubscriber.asDisposable;
};