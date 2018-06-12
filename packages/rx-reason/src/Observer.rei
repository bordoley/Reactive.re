type t('a);

type observer('a) = t('a);

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let complete: (option(exn), t('a)) => unit;
  let completeWithResult: (option(exn), t('a)) => bool;
  let isStopped: t('a) => bool;
  let next: ('a, t('a)) => unit;
  let toObserver: t('a) => observer('a);
};

include S1 with type t('a) := t('a);

let create:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

let createAutoDisposing:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

let disposed: t('a);