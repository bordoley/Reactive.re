open Functions.Operators;

let identity: Operator.t('a, 'a) = observer => observer;

let map = (mapper: 'a => 'b) : Operator.t('a, 'b) =>
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.createWithCallbacks(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let mapped =
              try (mapper(next)) {
              | exn =>
                observer |> Observer.complete(~exn=Some(exn));
                outerDisposable^ |> Disposable.dispose;
                Functions.returnUnit();
              };
            observer |> Observer.next(mapped);
          }),
        ~onComplete=exn => observer |> Observer.complete(~exn),
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let mapTo = (value: 'b) : Operator.t('a, 'b) => map((_) => value);

let doOnNext = (onNext: 'a => unit) : Operator.t('a, 'a) =>
  map(next => {
    onNext(next);
    next;
  });

let keep = (predicate: 'a => bool) : Operator.t('a, 'a) =>
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.createWithCallbacks(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let shouldKeep =
              try (predicate(next)) {
              | exn =>
                observer |> Observer.complete(~exn=Some(exn));
                outerDisposable^ |> Disposable.dispose;
                Functions.returnUnit();
              };
            if (shouldKeep) {
              observer |> Observer.next(next);
            };
          }),
        ~onComplete=exn => observer |> Observer.complete(~exn),
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

exception EmptyException;

let first: Operator.t('a, 'a) =
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.createWithCallbacks(
        ~onNext=
          next => {
            observer |> Observer.next(next);
            observer |> Observer.complete;
            outerDisposable^ |> Disposable.dispose;
          },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(_) => exn
              | _ => Some(EmptyException)
              };
            observer |> Observer.complete(~exn);
          },
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let firstOrNone = observer : Observer.t('a) => {
  let outerDisposable = ref(Disposable.disposed);
  let outerObserver =
    Observer.createWithCallbacks(
      ~onNext=
        next => {
          observer |> Observer.next(Some(next));
          observer |> Observer.complete;
          outerDisposable^ |> Disposable.dispose;
        },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException) =>
              observer |> Observer.next(None);
              None;
            | Some(_) => exn
            | _ =>
              observer |> Observer.next(None);
              exn;
            };
          observer |> Observer.complete(~exn);
        },
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
    );
  outerDisposable := outerObserver |> Observer.toDisposable;
  outerObserver;
};

let maybeFirst: Operator.t('a, 'a) =
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.createWithCallbacks(
        ~onNext=
          next => {
            observer |> Observer.next(next);
            observer |> Observer.complete;
            outerDisposable^ |> Disposable.dispose;
          },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(EmptyException) => None
              | _ => exn
              };
            observer |> Observer.complete(~exn);
          },
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let last: Operator.t('a, 'a) =
  observer => {
    let last = MutableOption.empty();
    Observer.createWithCallbacks(
      ~onNext=next => MutableOption.set(next, last),
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(_) => exn
            | _ =>
              if (MutableOption.isEmpty(last)) {
                Some(EmptyException);
              } else {
                let lastValue = MutableOption.firstOrRaise(last);
                observer |> Observer.next(lastValue);
                None;
              }
            };
          observer |> Observer.complete(~exn);
        },
      ~onDispose=() => {
        MutableOption.unset(last);
        observer |> Observer.toDisposable |> Disposable.dispose
      },
    );
  };

let lastOrNone: Operator.t('a, 'a) =
  observer => {
    let last = ref(None);
    Observer.createWithCallbacks(
      ~onNext=next => last := next,
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException) =>
              observer |> Observer.next(None);
              None;
            | Some(_) => exn
            | _ =>
              observer |> Observer.next(last^);
              last := None;
              exn;
            };
          observer |> Observer.complete(~exn);
        },
      ~onDispose=() => {
        last := None;
        observer |> Observer.toDisposable |> Disposable.dispose
      },
    );
  };

let maybeLast: Operator.t('a, 'a) =
  observer => {
    let last = MutableOption.empty();
    Observer.createWithCallbacks(
      ~onNext=next => MutableOption.set(next, last),
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException) => None
            | Some(_) => exn
            | _ =>
              if (MutableOption.isNotEmpty(last)) {
                let lastValue = MutableOption.firstOrRaise(last);
                MutableOption.unset(last);
                observer |> Observer.next(lastValue);
              };
              exn;
            };
          observer |> Observer.complete(~exn);
        },
      ~onDispose=() => {
        MutableOption.unset(last);
        observer |> Observer.toDisposable |> Disposable.dispose
      },
    );
  };

let scan =
    (scanner: ('acc, 'a) => 'acc, initialValue: 'acc)
    : Operator.t('a, 'acc) =>
  observer => {
    let acc = ref(initialValue);
    let mapper = next => {
      let prevAcc = acc^;
      let nextAcc = scanner(prevAcc, next);
      acc := nextAcc;
      nextAcc;
    };
    observer |> Observer.next(initialValue);
    map(mapper, observer);
  };

let distinctUntilChanged =
    (~comparer=Functions.referenceEquality)
    : Operator.t('a, 'a) => {
  let shouldUpdate = (a, b) => ! comparer(a, b);
  observer => {
    let state = MutableOption.empty();
    let predicate = next => MutableOption.setIf(shouldUpdate, next, state);
    keep(predicate, observer);
  };
};

let switch_: Operator.t(Observable.t('a), 'a) =
  observer => {
    let innerSubscription = ref(Disposable.disposed);
    let onComplete = exn => {
      observer |> Observer.complete(~exn);
      innerSubscription^ |> Disposable.dispose;
    };
    let innerObserver =
      Observer.create(
        ~onNext=next => observer |> Observer.next(next),
        ~onComplete=
          exn =>
            switch (exn) {
            | Some(_) => onComplete(exn)
            | None => innerSubscription^ |> Disposable.dispose
            },
        (),
      );
    let onNext = next =>
      try (
        {
          innerSubscription^ |> Disposable.dispose;
          innerSubscription :=
            next |> Observable.subscribeObserver(innerObserver);
        }
      ) {
      | exn => onComplete(Some(exn))
      };
    Observer.createWithCallbacks(
      ~onComplete,
      ~onNext,
      ~onDispose=() => {
        innerObserver |> Observer.toDisposable |> Disposable.dispose;
        innerSubscription^ |> Disposable.dispose;
        innerSubscription := Disposable.disposed;
      },
    );
  };

let switchMap = (mapper: 'a => Observable.t('b)) : Operator.t('a, 'b) =>
  map(mapper) << switch_;

/* FIXME: Should define a sane default DelayScheduler */
let debounceTime =
    (~scheduler: DelayScheduler.t, duration: float)
    : Operator.t('a, 'a) =>
  observer => {
    let lastValue = MutableOption.empty();
    let debounceSubscription = ref(Disposable.disposed);
    let clearDebounce = () => {
      let currentDebounceSubscription = debounceSubscription^;
      currentDebounceSubscription |> Disposable.dispose;
      debounceSubscription := Disposable.disposed;
    };
    let debouncedNext = () => {
      clearDebounce();
      if (MutableOption.isNotEmpty(lastValue)) {
        let next = lastValue^;
        MutableOption.unset(lastValue);
        observer |> Observer.next(next);
      };
      Disposable.disposed;
    };
    Observer.createWithCallbacks(
      ~onComplete=
        exn => {
          switch (exn) {
          | Some(_) => clearDebounce()
          | None => debouncedNext() |> ignore
          };
          observer |> Observer.complete(~exn);
        },
      ~onNext=
        next => {
          clearDebounce();
          lastValue := next;
          debounceSubscription := scheduler(~delay=duration, debouncedNext);
        },
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
    );
  };
/* bufferCount */
/* BufferSkipCount */
/* BufferTime */
/* withLatestFrom */