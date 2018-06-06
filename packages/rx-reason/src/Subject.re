type t('a) = {
  observer: Observer.t('a),
  observable: Observable.t('a),
};

exception DisposedException;

let toObserver = ({observer}: t('a)) : Observer.t('a) => observer;

let toObservable = ({observable}: t('a)) : Observable.t('a) => observable;

let createWithCallbacks =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      ~onDispose=Functions.alwaysUnit,
      ~onSubscribe=(~onNext as _, ~onComplete as _) => (),
      (),
    )
    : t('a) => {
  let subscribers = ref(CopyOnWriteArray.empty());
  let observer =
    Observer.create(
      ~onComplete=
        exn => {
          let currentSubscribers = subscribers^;
          onComplete(exn);
          currentSubscribers
          |> CopyOnWriteArray.forEach(((_, onComplete)) => onComplete(exn));
        },
      ~onNext=
        next => {
          onNext(next);
          let currentSubscribers = subscribers^;
          currentSubscribers
          |> CopyOnWriteArray.forEach(((onNext, _)) => onNext(next));
          ();
        },
      ~onDispose=
        () => {
          onDispose();
          subscribers := CopyOnWriteArray.empty();
        },
    );

  let raiseIfDisposed = (disposable: Disposable.t) : unit =>
    if (Disposable.isDisposed(disposable)) {
      raise(DisposedException);
    };
  let observable =
    Observable.create((~onNext, ~onComplete) => {
      observer |> Observer.toDisposable |> raiseIfDisposed;
      let currentSubscribers = subscribers^;
      let subscriber = (onNext, onComplete);
      subscribers :=
        currentSubscribers |> CopyOnWriteArray.addLast(subscriber);
      onSubscribe(~onNext, ~onComplete);
      Disposable.create(() => {
        let currentSubscribers = subscribers^;
        subscribers :=
          currentSubscribers
          |> CopyOnWriteArray.findAndRemove(
               Functions.referenceEquality,
               subscriber,
             );
      });
    });
  {observer, observable};
};

let create = () => createWithCallbacks();

let createWithReplayBuffer = (maxBufferCount: int) : t('a) => {
  let buffer = ref(CopyOnWriteArray.empty());
  let completed = ref(false);
  let completedValue = ref(None);

  createWithCallbacks(
    ~onNext=
      next => {
        let currentBuffer = buffer^;
        let nextBuffer =
          if (CopyOnWriteArray.count(currentBuffer) === maxBufferCount) {
            currentBuffer |> CopyOnWriteArray.removeAt(0);
          } else {
            currentBuffer;
          };
        buffer := nextBuffer |> CopyOnWriteArray.addLast(next);
      },
    ~onComplete=
      exn => {
        completed := true;
        completedValue := exn;
      },
    ~onDispose=() => buffer := CopyOnWriteArray.empty(),
    ~onSubscribe=
      (~onNext, ~onComplete) => {
        let currentBuffer = buffer^;
        CopyOnWriteArray.forEach(onNext, currentBuffer);
        if (completed^) {
          onComplete(completedValue^);
        };
      },
    (),
  );
};

let shareInternal =
    (~createSubject, source: Observable.t('a))
    : Observable.t('a) => {
  let subject = MutableOption.create();
  let sourceSubscription = ref(Disposable.disposed);
  let refCount = ref(0);
  let reset = () => {
    sourceSubscription^ |> Disposable.dispose;
    subject |> MutableOption.unset;
    refCount := 0;
  };
  Observable.create((~onNext, ~onComplete) => {
    let currentSubject = {
      if (refCount^ === 0) {
        MutableOption.set(createSubject(), subject);
      };
      subject |> MutableOption.get;
    };
    let subjectObservable = toObservable(currentSubject);
    let subjectObserver = toObserver(currentSubject);
    let observerSubscription =
      subjectObservable |> Observable.subscribeWithCallbacks(~onNext, ~onComplete);
    if (refCount^ === 0) {
      sourceSubscription :=
        source
        |> Observable.subscribeWithCallbacks(
             ~onNext=next => subjectObserver |> Observer.next(next),
             ~onComplete=
               exn => {
                 subjectObserver |> Observer.complete(exn);
                 reset();
               },
           );
    };
    if (subjectObserver |> Observer.toDisposable |> Disposable.isDisposed) {
      /* The source completed synchronously and reset */
      observerSubscription |> Disposable.dispose;
      Disposable.disposed;
    } else {
      refCount := refCount^ + 1;
      Disposable.create(() => {
        refCount := refCount^ > 0 ? refCount^ - 1 : 0;
        if (refCount^ === 0) {
          reset();
        };
        observerSubscription |> Disposable.dispose;
      });
    };
  });
};

let share = obs => shareInternal(~createSubject=create, obs);

let shareWithReplayBuffer = (count: int, obs) =>
  shareInternal(~createSubject=() => createWithReplayBuffer(count), obs);