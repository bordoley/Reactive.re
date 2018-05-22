type t('a);

let create: (Observer.t('a) => Disposable.t) => t('a);

let defer: (unit => t('a)) => t('a);

let empty: (~scheduler: Scheduler.t=?, unit) => t('a);

let first: Observable.t('a) => t('a);

let last: Observable.t('a) => t('a);

let lift: (Operator.t('a, 'b), t('a)) => t('b);

let never: unit => t('a);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let subscribe:
  (~onComplete: option(exn) => unit=?, ~onNext: 'a => unit=?, t('a)) =>
  Disposable.t;

let subscribeObserver: (Observer.t('a), t('a)) => Disposable.t;

let toObservable: t('a) => Observable.t('a);