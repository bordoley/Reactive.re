type t('a) = ObservableSource.t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);

  let publish:
    (
      ~onNext: 'a => unit=?,
      ~onComplete: option(exn) => unit=?,
      t('a),
      unit
    ) =>
    Disposable.t;

  let publishTo:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    CompositeDisposable.t;

  let subscribe1:
    (
      ~onNext: ('ctx0, 'a) => unit=?,
      ~onComplete: ('ctx0, option(exn)) => unit=?,
      'ctx0,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe2:
    (
      ~onNext: ('ctx0, 'ctx1, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe3:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    CompositeDisposable.t;

  let subscribeWith1:
    (
      ~onNext: ('ctx0, 'a) => unit,
      ~onComplete: ('ctx0, option(exn)) => unit,
      'ctx0,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith2:
    (
      ~onNext: ('ctx0, 'ctx1, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
      'ctx0,
      'ctx1,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith3:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      t('a)
    ) =>
    CompositeDisposable.t;
};

let asObservable = Functions.identity;
let combineLatest2 = CombineLatestObservableSource.combineLatest2;
let combineLatest3 = CombineLatestObservableSource.combineLatest3;
let combineLatest4 = CombineLatestObservableSource.combineLatest4;
let combineLatest5 = CombineLatestObservableSource.combineLatest5;
let combineLatest6 = CombineLatestObservableSource.combineLatest6;
let combineLatest7 = CombineLatestObservableSource.combineLatest7;
let concat = ConcatObservableSource.concat;
let create = ObservableSource.create;
let debounce = DebounceOperator.lift;
let defaultIfEmpty = DefaultIfEmptyOperator.lift;
let distinctUntilChanged = DistinctUntilChangedOperator.lift;
let every = EveryOperator.lift;
let exhaust = ExhaustOperator.lift;
let find = FindOperator.lift;
let first = FirstOperator.lift;
let firstOrNone = FirstOrNoneOperator.lift;
let ignoreElements = IgnoreElementsOperator.lift;
let isEmpty = IsEmptyOperator.lift;
let keep = KeepOperator.lift;
let last = LastOperator.lift;
let lastOrNone = LastOrNoneOperator.lift;
let lift = ObservableSource.lift;
let map = MapOperator.lift;
let mapTo = MapToOperator.lift;
let materialize = MaterializeOperator.lift;
let maybeFirst = MaybeFirstOperator.lift;
let maybeLast = MaybeLastOperator.lift;
let merge = MergeObservableSource.merge;
let never = ObservableSource.never;
let none = NoneOperator.lift;
let observe = ObserveOperator.lift;
let observeOn = ObserveOnOperator.lift;
let onComplete = OnCompleteOperator.lift;
let onNext = OnNextOperator.lift;
let onSubscribe = OnSubscribeOperator.lift;
let publish = ObservableSource.publish;
let publishTo = ObservableSource.publishTo;
let raise = ObservableSource.raise;
let scan = ScanOperator.lift;
let some = SomeOperator.lift;
let switch_ = SwitchOperator.lift;
let subscribeSubscriber = ObservableSource.subscribeSubscriber;
let subscribeWith = ObservableSource.subscribeWith;
let subscribeWith1 = ObservableSource.subscribeWith1;
let subscribeWith2 = ObservableSource.subscribeWith2;
let subscribeWith3 = ObservableSource.subscribeWith3;
let subscribe = ObservableSource.subscribe;
let subscribe1 = ObservableSource.subscribe1;
let subscribe2 = ObservableSource.subscribe2;
let subscribe3 = ObservableSource.subscribe3;
let synchronize = SynchronizeOperator.lift;
let timeout = TimeoutOperator.lift;
let withLatestFrom = WithLatestFromOperator.lift;

let defer = (f: unit => t('a)) : t('a) =>
  create(subscriber => f() |> subscribeSubscriber(subscriber));

let empty = (~scheduler=Scheduler.immediate, ()) =>
  scheduler === Scheduler.immediate ?
    create(subscriber => subscriber |> Subscriber.complete) :
    create(subscriber => {
      let schedulerSubscription =
        scheduler(() => {
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        });
      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    });

let ofAbsoluteTimeNotifications =
    (
      ~scheduler as {now, scheduleWithDelay}: ClockScheduler.t,
      notifications: list((float, Notification.t('a))),
    )
    : t('a) =>
  create(subscriber => {
    let rec loop = lst =>
      switch (lst) {
      | [(time, notif), ...tail] =>
        let delay = time -. now();
        if (delay > 0.0) {
          scheduleWithDelay(
            delay,
            () => {
              subscriber |> Subscriber.notify(notif);
              loop(tail);
            },
          );
        } else {
          loop(tail);
        };
      | [] => Disposable.disposed
      };

    let loopSubscription = loop(notifications);
    subscriber |> Subscriber.addDisposable(loopSubscription) |> ignore;
  });

let ofList = (~scheduler=Scheduler.immediate, list: list('a)) : t('a) =>
  scheduler === Scheduler.immediate ?
    create(subscriber => {
      let rec loop = list =>
        switch (list) {
        | [hd, ...tail] =>
          subscriber |> Subscriber.next(hd);
          loop(tail);
        | [] => subscriber |> Subscriber.complete
        };
      loop(list);
    }) :
    create(subscriber => {
      let rec loop = (list, ()) =>
        switch (list) {
        | [hd] =>
          subscriber |> Subscriber.next(hd);
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        | [hd, ...tail] =>
          subscriber |> Subscriber.next(hd);
          scheduler(loop(tail));
        | [] =>
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        };

      let schedulerSubscription = scheduler(loop(list));
      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    });

let ofNotifications =
    (
      ~scheduler as schedule=Scheduler.immediate,
      notifications: list(Notification.t('a)),
    )
    : t('a) =>
  create(
    schedule === Scheduler.immediate ?
      subscriber => {
        let rec loop = list =>
          switch (list) {
          | [hd, ...tail] =>
            subscriber |> Subscriber.notify(hd);
            loop(tail);
          | [] => ()
          };
        loop(notifications);
      } :
      (
        subscriber => {
          let rec loop = (list, ()) =>
            switch (list) {
            | [hd] =>
              subscriber |> Subscriber.notify(hd);
              Disposable.disposed;
            | [hd, ...tail] =>
              subscriber |> Subscriber.notify(hd);
              schedule(loop(tail));
            | [] => Disposable.disposed
            };
          let schedulerSubscription = schedule(loop(notifications));
          subscriber
          |> Subscriber.addDisposable(schedulerSubscription)
          |> ignore;
        }
      ),
  );

let ofRelativeTimeNotifications =
    (
      ~scheduler as schedule: DelayScheduler.t,
      notifications: list((float, Notification.t('a))),
    )
    : t('a) =>
  create(subscriber => {
    let rec loop = (lst, previousDelay) =>
      switch (lst) {
      | [(delay, notif), ...tail] =>
        schedule(
          max(0.0, delay -. previousDelay),
          () => {
            subscriber |> Subscriber.notify(notif);
            loop(tail, delay);
          },
        )
      | [] => Disposable.disposed
      };

    let loopSubscription = loop(notifications, 0.0);
    subscriber |> Subscriber.addDisposable(loopSubscription) |> ignore;
  });

let ofValue = (~scheduler=Scheduler.immediate, value: 'a) : t('a) =>
  scheduler === Scheduler.immediate ?
    create(subscriber => {
      subscriber |> Subscriber.next(value);
      subscriber |> Subscriber.complete;
    }) :
    create(subscriber => {
      let schedulerSubscription =
        scheduler(() => {
          subscriber |> Subscriber.next(value);
          scheduler(() => {
            subscriber |> Subscriber.complete;
            Disposable.disposed;
          });
        });

      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    });

let repeat = (~predicate=Functions.alwaysTrue1, observable) =>
  RepeatOperator.lift(
    fun
    | None => predicate()
    | Some(_) => false,
    observable,
  );

let retry = (~predicate=Functions.alwaysTrue1, observable) =>
  RepeatOperator.lift(
    fun
    | None => false
    | Some(exn) => predicate(exn),
    observable,
  );

let startWithList =
    (~scheduler=Scheduler.immediate, values: list('a), observable: t('a)) =>
  concat([ofList(~scheduler, values), observable]);

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: t('a)) =>
  concat([ofValue(~scheduler, value), observable]);

let subscribeOn = (scheduler: Scheduler.t, observable: t('a)) : t('a) =>
  create(subscriber => {
    let schedulerSubscription =
      scheduler(() => {
        observable |> subscribeSubscriber(subscriber);
        Disposable.disposed;
      });
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  });

let toList = observable =>
  observable
  |> scan((acc, next) => [next, ...acc], [])
  |> last
  |> map(List.rev);