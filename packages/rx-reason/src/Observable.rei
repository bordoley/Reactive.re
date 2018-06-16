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

/**
 * Combines 2 source Observables, emitting items which aggregate the latest 
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the 
 * provided selector function.
 */
let combineLatest2: (~selector: ('a, 'b) => 'c, t('a), t('b)) => t('c);

/**
 * Combines 3 source Observables, emitting items which aggregate the latest 
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the 
 * provided selector function.
 */
let combineLatest3:
  (~selector: ('a, 'b, 'c) => 'd, t('a), t('b), t('c)) => t('d);

/**
 * Combines 4 source Observables, emitting items which aggregate the latest 
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the 
 * provided selector function.
 */
let combineLatest4:
  (~selector: ('a, 'b, 'c, 'd) => 'e, t('a), t('b), t('c), t('d)) => t('e);

/**
 * Combines 5 source Observables, emitting items which aggregate the latest 
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the 
 * provided selector function.
 */
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

/**
 * Combines 6 source Observables, emitting items which aggregate the latest 
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the 
 * provided selector function.
 */
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

/**
 * Combines 7 source Observables, emitting items which aggregate the latest 
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the 
 * provided selector function.
 */
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

/**
 * Returns an Observable which concatenates elements of each provided Observable
 * into a single sequence of elements without interleaving them.
 */
let concat: (~scheduler: Scheduler.t=?, list(t('a))) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create:
  ((~onNext: 'a => unit, ~onComplete: option(exn) => unit) => Disposable.t) =>
  t('a);

/**
 * Returns an Observable that calls an Observable factory to 
 * create a new Observable for each subscription.
 */
let defer: (unit => t('a)) => t('a);

/**
 * Returns an Observable that emits no items and immediately completes.
 */
let empty: (~scheduler: Scheduler.t=?, unit) => t('a);

/**
 * Returns an Observable that applies the Operator function to the
 * source Observable's notifications.
 */
let lift: (Operator.t('a, 'b), t('a)) => t('b);

/**
 * Returns an Observable that merges items emitted by the source Observables,
 * interleaving the items emitted by each Observable.
 */
let merge: list(t('a)) => t('a);

/**
 * Returns an Observable the emits no values and never completes.
 */
let never: t('a);

/** 
 * Returns an Observable which emits the specified notifications at the 
 * specified absolutes times using the supplied scheduler.
 */
let ofAbsoluteTimeNotifications:
  (~scheduler: ClockScheduler.t, list((float, Notification.t('a)))) =>
  t('a);

/**
 * Returns an Observable that emits the given items and then completes.
 */ 
let ofList: (~scheduler: Scheduler.t=?, list('a)) => t('a);

/** 
 * Returns an Observable which emits the specified notifications
 * using the supplied scheduler.
 */
let ofNotifications:
  (~scheduler: Scheduler.t=?, list(Notification.t('a))) => t('a);

/** 
 * Returns an Observable which emits the specified notifications at the 
 * specified relative times using the supplied scheduler.
 */
let ofRelativeTimeNotifications:
  (~scheduler: DelayScheduler.t, list((float, Notification.t('a)))) =>
  t('a);

 /**
  * Returns an Observable that emits the given item and then completes.
  */ 
let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * calling the specified function when subscribed to, and
 * disposing the returned Disposable when disposed.
 */
let onSubscribe: (unit => Disposable.t, t('a)) => t('a);

/**
 * Returns an Observable that completes with the specified exception
 * when subscribed to.
 */
let raise: (~scheduler: Scheduler.t=?, exn) => t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes with an exception and 
 * the predicate returns true for that specific exception.
 */
let retry: (exn => bool, t('a)) => t('a);

/**
 * Returns an Observable that emits the specified items before it 
 * begins to emit items emitted by the source Observable.
 */
let startWithList: (~scheduler: Scheduler.t=?, list('a), t('a)) => t('a);

/**
 * Returns an Observable that emits a specified item before it 
 * begins to emit items emitted by the source Observable.
 */
let startWithValue: (~scheduler: Scheduler.t=?, 'a, t('a)) => t('a);

/**
 * Returns an Observable which subscribes to the provided source Observable 
 * on the specified scheduler.
 */
let subscribeOn: (Scheduler.t, t('a)) => t('a);