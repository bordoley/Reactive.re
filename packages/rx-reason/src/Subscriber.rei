/**
 * Represents the underlying subscription to an Observable and it's callbacks.
 * Subscribers are a specialized type, primarily used in the implementation of
 * Operators. Normal use cases, such as subscribing to an Observable, should use
 * callback functions instead.
 */;
type t('a);

include CompositeDisposable.S1 with type t('a) := t('a);
include ObserverLike.S1 with type t('a) := t('a);

/**
 * Construct a new Subscriber with the provided callbacks which automatically disposes
 * itself when completed.
 * */
let createAutoDisposing:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => t('a);

let createAutoDisposing1:
  (
    ~onNext: ('ctx0, 'a) => unit,
    ~onComplete: ('ctx0, option(exn)) => unit,
    'ctx0
  ) =>
  t('a);

let createAutoDisposing2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
    'ctx0,
    'ctx1
  ) =>
  t('a);

let createAutoDisposing3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2
  ) =>
  t('a);

let createAutoDisposing4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3
  ) =>
  t('a);

let createAutoDisposing5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4
  ) =>
  t('a);

let createAutoDisposing6:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5
  ) =>
  t('a);

let createAutoDisposing7:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) => unit,
    ~onComplete: (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   option(exn)
                 ) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6
  ) =>
  t('a);

/** Construct a new Subscriber with the provided callbacks that delegates its disposal to another Subscriber. */
let delegate:
  (
    ~onNext: (t('b), 'a) => unit,
    ~onComplete: (t('b), option(exn)) => unit,
    t('b)
  ) =>
  t('a);

let delegate1:
  (
    ~onNext: ('ctx0, t('b), 'a) => unit,
    ~onComplete: ('ctx0, t('b), option(exn)) => unit,
    'ctx0,
    t('b)
  ) =>
  t('a);

let delegate2:
  (
    ~onNext: ('ctx0, 'ctx1, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    t('b)
  ) =>
  t('a);

let delegate3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    t('b)
  ) =>
  t('a);

let delegate4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t('b)
  ) =>
  t('a);

let delegate5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t('b)
  ) =>
  t('a);

let delegate6:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('b), 'a) => unit,
    ~onComplete: (
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
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    t('b)
  ) =>
  t('a);

let delegate7:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('b), 'a) =>
             unit,
    ~onComplete: (
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
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6,
    t('b)
  ) =>
  t('a);

let delegateOnComplete: (t('a), option(exn)) => unit;
let delegateOnComplete1: ('ctx0, t('a), option(exn)) => unit;
let delegateOnComplete2: ('ctx0, 'ctx1, t('a), option(exn)) => unit;
let delegateOnComplete3: ('ctx0, 'ctx1, 'ctx2, t('a), option(exn)) => unit;
let delegateOnComplete4:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), option(exn)) => unit;
let delegateOnComplete5:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), option(exn)) => unit;
let delegateOnComplete6:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('a), option(exn)) => unit;
let delegateOnComplete7:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('a), option(exn)) =>
  unit;

let delegateOnNext: (t('a), 'a) => unit;
let delegateOnNext1: ('ctx0, t('a), 'a) => unit;
let delegateOnNext2: ('ctx0, 'ctx1, t('a), 'a) => unit;
let delegateOnNext3: ('ctx0, 'ctx1, 'ctx2, t('a), 'a) => unit;
let delegateOnNext4: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), 'a) => unit;
let delegateOnNext5: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), 'a) => unit;
let delegateOnNext6:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('a), 'a) => unit;
let delegateOnNext7:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('a), 'a) => unit;

/**
 * A disposed Subscriber instance.
 */
let disposed: t('a);

/** Returns true if the Observer has been completed or disposed. */
let isStopped: t('a) => bool;