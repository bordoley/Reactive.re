/**
 * A provider of push-based notifications.
 */;
type t('a);

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

  let publishObserver: (Observer.t('a), t('a), unit) => Disposable.t;

  let publishWithCallbacks:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    Disposable.t;

  let subscribeObserver: (Observer.t('a), t('a)) => Disposable.t;

  let subscribeWithCallbacks:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    Disposable.t;
};

include S1 with type t('a) := t('a);

let combineLatest2: (~selector: ('a, 'b) => 'c, t('a), t('b)) => t('c);

let combineLatest3:
  (~selector: ('a, 'b, 'c) => 'd, t('a), t('b), t('c)) => t('d);

let combineLatest4:
  (~selector: ('a, 'b, 'c, 'd) => 'e, t('a), t('b), t('c), t('d)) => t('e);

let combineLatest5:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e) => 'f,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e)
  ) =>
  t('f);

let combineLatest6:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e, 'f) => 'g,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e),
    t('f)
  ) =>
  t('g);

let combineLatest7:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e, 'f, 'g) => 'h,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e),
    t('f),
    t('g)
  ) =>
  t('h);

let concat: (~scheduler: Scheduler.t=?, list(t('a))) => t('a);

let create:
  ((~onNext: 'a => unit, ~onComplete: option(exn) => unit) => Disposable.t) =>
  t('a);

let defer: (unit => t('a)) => t('a);

let empty: (~scheduler: Scheduler.t=?, unit) => t('a);

let lift: (Operator.t('a, 'b), t('a)) => t('b);

let merge: list(t('a)) => t('a);

let never: t('a);

let ofAbsoluteTimeNotifications:
  (~scheduler: ClockScheduler.t, list((float, Notification.t('a)))) =>
  t('a);

let ofList: (~scheduler: Scheduler.t=?, list('a)) => t('a);

let ofNotifications:
  (~scheduler: Scheduler.t=?, list(Notification.t('a))) => t('a);

let ofRelativeTimeNotifications:
  (~scheduler: DelayScheduler.t, list((float, Notification.t('a)))) =>
  t('a);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let onSubscribe: (unit => Disposable.t, t('a)) => t('a);

let raise: (~scheduler: Scheduler.t=?, exn) => t('a);

let retry: (exn => bool, t('a)) => t('a);

let startWithList: (~scheduler: Scheduler.t=?, list('a), t('a)) => t('a);

let startWithValue: (~scheduler: Scheduler.t=?, 'a, t('a)) => t('a);

let subscribeOn: (Scheduler.t, t('a)) => t('a);