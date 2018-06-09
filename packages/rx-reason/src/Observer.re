type t('a) = {
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: Disposable.t,
};

let create = (~onNext, ~onComplete, ~onDispose) : t('a) => {
  let isStopped = ref(false);
  {
    isStopped,
    onComplete,
    onNext,
    disposable:
      Disposable.create(() => {
        Volatile.write(true, isStopped);
        onDispose();
      }),
  };
};

let createAutoDisposing = (~onNext, ~onComplete, ~onDispose) : t('a) => {
  let isStopped = ref(false);
  let disposable =
    Disposable.create(() => {
      Volatile.write(true, isStopped);
      onDispose();
    });

  {
    isStopped,
    onComplete: exn => {
      try (onComplete(exn)) {
      | exn =>
        disposable |> Disposable.dispose;
        raise(exn);
      };
      disposable |> Disposable.dispose;
    },
    onNext: next =>
      try (onNext(next)) {
      | exn =>
        disposable |> Disposable.dispose;
        raise(exn);
      },
    disposable,
  };
};

let completeWithResult =
    (exn: option(exn), {isStopped, onComplete}: t('a))
    : bool => {
  let shouldComplete = ! Interlocked.exchange(true, isStopped);
  if (shouldComplete) {
    onComplete(exn);
  };
  shouldComplete;
};

let complete = (exn: option(exn), observer: t('a)) : unit =>
  observer |> completeWithResult(exn) |> ignore;

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let disposedIsStopped = ref(true);
let disposed = {
  isStopped: disposedIsStopped,
  onComplete: Functions.alwaysUnit,
  onNext: Functions.alwaysUnit,
  disposable: Disposable.disposed,
};

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let isStopped = ({isStopped}) => Volatile.read(isStopped);

let next = (next: 'a, {onNext, isStopped}: t('a)) : unit => {
  let isStopped = Volatile.read(isStopped);
  if (! isStopped) {
    onNext(next);
  };
};

let toDisposable = ({disposable}: t('a)) : Disposable.t => disposable;