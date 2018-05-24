type t('a);

let create: (Observer.t('a) => Disposable.t) => t('a);

let defer: (unit => t('a)) => t('a);

let empty: (~scheduler: Rx.Scheduler.t=?, unit) => t('a);

let first: Observable.t('a) => t('a);

let last: Observable.t('a) => t('a);

let liftFirst: (Operator.t('a, 'b), t('a)) => t('b);

let liftLast: (Operator.t('a, 'b), t('a)) => t('b);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let reduce: (('acc, 'a) => 'acc, 'acc, Observable.t('a)) => t('acc);

let subscribe:
  (~onComplete: option(exn) => unit=?, ~onNext: 'a => unit=?, t('a)) =>
  Disposable.t;

let subscribeObserver: (Observer.t('a), t('a)) => Disposable.t;

let toObservable: t('a) => Observable.t('a);

let toSingle: t('a) => Single.t(option('a));