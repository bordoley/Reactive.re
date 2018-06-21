type t('a) = {
  subscriber: Subscriber.t('a),
  observable: Observable.t('a),
  disposable: Disposable.t,
};

let disposed = {
  subscriber: Subscriber.disposed,
  observable: Observable.never,
  disposable: Disposable.disposed,
};

let asDisposable = ({disposable}) => disposable;

let asObservable = ({observable}) => observable;

let complete = (~exn=?, {subscriber}) =>
  subscriber |> Subscriber.complete(~exn?);

let completeWithResult = (~exn=?, {subscriber}) =>
  subscriber |> Subscriber.completeWithResult(~exn?);

let delegateOnComplete = ({subscriber}, exn) =>
  subscriber |> Subscriber.complete(~exn?);

let delegateOnComplete1 = (_, {subscriber}, exn) =>
  subscriber |> Subscriber.complete(~exn?);

let delegateOnComplete2 = (_, _, {subscriber}, exn) =>
  subscriber |> Subscriber.complete(~exn?);

let delegateOnComplete3 = (_, _, _, {subscriber}, exn) =>
  subscriber |> Subscriber.complete(~exn?);

let delegateOnComplete4 = (_, _, _, _, {subscriber}, exn) =>
  subscriber |> Subscriber.complete(~exn?);

let delegateOnNext = ({subscriber}, next) =>
  subscriber |> Subscriber.next(next);

let delegateOnNext1 = (_, {subscriber}, next) =>
  subscriber |> Subscriber.next(next);

let delegateOnNext2 = (_, _, {subscriber}, next) =>
  subscriber |> Subscriber.next(next);

let delegateOnNext3 = (_, _, _, {subscriber}, next) =>
  subscriber |> Subscriber.next(next);

let delegateOnNext4 = (_, _, _, _, {subscriber}, next) =>
  subscriber |> Subscriber.next(next);

let dispose = self => self |> asDisposable |> Disposable.dispose;

let isDisposed = self => self |> asDisposable |> Disposable.isDisposed;

let next = (next, {subscriber}) => subscriber |> Subscriber.next(next);

let notify = (notif, {subscriber}) =>
  subscriber |> Subscriber.notify(notif);

let raiseIfDisposed = self =>
  self |> asDisposable |> Disposable.raiseIfDisposed;

let publishTo = (~onNext, ~onComplete, subject) =>
  subject |> asObservable |> Observable.publishTo(~onNext, ~onComplete);

let publish =
    (
      ~onNext=Functions.alwaysUnit1,
      ~onComplete=Functions.alwaysUnit1,
      subject,
    ) =>
  subject |> publishTo(~onNext, ~onComplete);

let subscribeWith = (~onNext, ~onComplete, subject) =>
  subject |> asObservable |> Observable.subscribeWith(~onNext, ~onComplete);

let subscribeWith1 = (~onNext, ~onComplete, ctx0, subject) =>
  subject
  |> asObservable
  |> Observable.subscribeWith1(~onNext, ~onComplete, ctx0);

let subscribeWith2 = (~onNext, ~onComplete, ctx0, ctx1, subject) =>
  subject
  |> asObservable
  |> Observable.subscribeWith2(~onNext, ~onComplete, ctx0, ctx1);

let subscribeWith3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subject) =>
  subject
  |> asObservable
  |> Observable.subscribeWith3(~onNext, ~onComplete, ctx0, ctx1, ctx2);

let subscribe = (~onNext=?, ~onComplete=?, subject) =>
  subject |> asObservable |> Observable.subscribe(~onNext?, ~onComplete?);

let subscribe1 = (~onNext=?, ~onComplete=?, ctx0, subject) =>
  subject
  |> asObservable
  |> Observable.subscribe1(~onNext?, ~onComplete?, ctx0);

let subscribe2 = (~onNext=?, ~onComplete=?, ctx0, ctx1, subject) =>
  subject
  |> asObservable
  |> Observable.subscribe2(~onNext?, ~onComplete?, ctx0, ctx1);

let subscribe3 = (~onNext=?, ~onComplete=?, ctx0, ctx1, ctx2, subject) =>
  subject
  |> asObservable
  |> Observable.subscribe3(~onNext?, ~onComplete?, ctx0, ctx1, ctx2);

let create = {
  let teardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> CopyOnWriteArray.findAndRemove(
           Functions.referenceEquality,
           subscriber,
         );
  };

  let subscriberOnNext = (subscribers, next) => {
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.next(next)
       );
  };

  let subscriberOnComplete = (subscribers, exn) => {
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.complete(~exn?)
       );
  };

  let disposableTeardown = (subscriber, subscribers) => {
    subscriber |> Subscriber.dispose;
    subscribers := CopyOnWriteArray.empty();
  };

  () => {
    let subscribers = ref(CopyOnWriteArray.empty());

    let subscriber =
      Subscriber.createAutoDisposing1(
        ~onNext=subscriberOnNext,
        ~onComplete=subscriberOnComplete,
        subscribers,
      );

    let disposable =
      Disposable.create2(disposableTeardown, subscriber, subscribers);

    let observable =
      Observable.create(subscriber => {
        disposable |> Disposable.raiseIfDisposed;
        subscribers := subscribers^ |> CopyOnWriteArray.addLast(subscriber);

        subscriber
        |> Subscriber.addTeardown2(teardown, subscriber, subscribers)
        |> ignore;
      });

    {subscriber, observable, disposable};
  };
};

let createWithReplayBuffer = {
  let teardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> CopyOnWriteArray.findAndRemove(
           Functions.referenceEquality,
           subscriber,
         );
  };

  let subscriberOnNext = (maxBufferCount, buffer, _, _, subscribers, next) => {
    let currentBuffer = buffer^;
    let nextBuffer =
      if (CopyOnWriteArray.count(currentBuffer) === maxBufferCount) {
        currentBuffer |> CopyOnWriteArray.removeAt(0);
      } else {
        currentBuffer;
      };
    buffer := nextBuffer |> CopyOnWriteArray.addLast(next);

    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.next(next)
       );
  };

  let subscriberOnComplete =
      (_, _, completed, completedValue, subscribers, exn) => {
    completed := true;
    completedValue := exn;

    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.complete(~exn?)
       );
  };

  let disposableTeardown = (buffer, subscriber, subscribers) => {
    buffer := CopyOnWriteArray.empty();
    subscriber |> Subscriber.dispose;
    subscribers := CopyOnWriteArray.empty();
  };

  (maxBufferCount: int) => {
    /* This is fine for small buffers, eg. < 32
     * Ideally we'd use something like an Immutable.re vector
     * for large replay buffers.
     *
     * We use a cow array in order to snapshot the events we replay
     * in case their is feedback in the event system.
     */
    let buffer = ref(CopyOnWriteArray.empty());
    let completed = ref(false);
    let completedValue = ref(None);

    let subscribers = ref(CopyOnWriteArray.empty());

    let subscriber =
      Subscriber.createAutoDisposing5(
        ~onNext=subscriberOnNext,
        ~onComplete=subscriberOnComplete,
        maxBufferCount,
        buffer,
        completed,
        completedValue,
        subscribers,
      );

    let disposable =
      Disposable.create3(disposableTeardown, buffer, subscriber, subscribers);

    let observable =
      Observable.create(subscriber => {
        disposable |> Disposable.raiseIfDisposed;
        subscribers := subscribers^ |> CopyOnWriteArray.addLast(subscriber);

        let currentBuffer = buffer^;
        CopyOnWriteArray.forEach(
          next => subscriber |> Subscriber.next(next),
          currentBuffer,
        );
        if (completed^) {
          let exn = completedValue^;
          subscriber |> Subscriber.complete(~exn?);
        };

        subscriber
        |> Subscriber.addTeardown2(teardown, subscriber, subscribers)
        |> ignore;
      });

    {subscriber, observable, disposable};
  };
};