let concat = ConcatObservable.concat;

let debounce = (~scheduler, dueTime, observable) =>
  observable |> RxObservable.lift(RxOperators.debounce(~scheduler, dueTime));

let defaultIfEmpty = (default, observable) =>
  observable |> RxObservable.lift(RxOperators.defaultIfEmpty(default));

let defer = DeferObservable.defer;

let delay = (~scheduler, delay, observable) =>
  observable |> RxObservable.lift(RxOperators.delay(~scheduler, delay));

let dematerialize = observable =>
  observable |> RxObservable.lift(RxOperators.dematerialize);

let distinctUntilChanged = (~equals=?, observable) =>
  observable |> RxObservable.lift(RxOperators.distinctUntilChanged(~equals?));

let empty = EmptyObservable.empty;

let every = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.every(predicate));

let exhaust = observable =>
  observable |> RxObservable.lift(RxOperators.exhaust);

let find = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.find(predicate));

let first = observable => observable |> RxObservable.lift(RxOperators.first);

let firstOrNone = observable =>
  observable |> RxObservable.lift(RxOperators.firstOrNone);

let ignoreElements = observable =>
  observable |> RxObservable.lift(RxOperators.ignoreElements);

let isEmpty = observable =>
  observable |> RxObservable.lift(RxOperators.isEmpty);

let keep = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.keep(predicate));

let last = observable => observable |> RxObservable.lift(RxOperators.last);

let lastOrNone = observable =>
  observable |> RxObservable.lift(RxOperators.lastOrNone);

let map = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.map(f));

let mapTo = (value, observable) =>
  observable |> RxObservable.lift(RxOperators.mapTo(value));

let materialize = observable =>
  observable |> RxObservable.lift(RxOperators.materialize);

let maybeFirst = observable =>
  observable |> RxObservable.lift(RxOperators.maybeFirst);

let maybeLast = observable =>
  observable |> RxObservable.lift(RxOperators.maybeLast);

let merge = MergeObservable.merge;

let none = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.none(predicate));

let observeOn = (scheduler, observable) =>
  observable |> RxObservable.lift(RxOperators.observeOn(scheduler));

let ofAbsoluteTimeNotifications = OfObservable.ofAbsoluteTimeNotifications;
let ofList = OfObservable.ofList;
let ofNotifications = OfObservable.ofNotifications;
let ofRelativeTimeNotifications = OfObservable.ofRelativeTimeNotifications;
let ofValue = OfObservable.ofValue;

let onComplete = (onComplete, observable) =>
  observable
  |> RxObservable.observe(~onNext=RxFunctions.alwaysUnit1, ~onComplete);

let onExn = (onExn, observable) =>
  observable
  |> RxObservable.observe(
       ~onNext=RxFunctions.alwaysUnit1,
       ~onComplete=
         fun
         | None => ()
         | Some(exn) => onExn(exn),
     );

let onNext = (onNext, observable) =>
  observable
  |> RxObservable.observe(~onNext, ~onComplete=RxFunctions.alwaysUnit1);

let onNext1 = (onNext, ctx0, observable) =>
  observable
  |> RxObservable.observe1(~onNext, ~onComplete=RxFunctions.alwaysUnit2, ctx0);

let onNext2 = (onNext, ctx0, ctx1, observable) =>
  observable
  |> RxObservable.observe2(~onNext, ~onComplete=RxFunctions.alwaysUnit3, ctx0, ctx1);

let onSubscribe = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.onSubscribe(f));

let raise = RaiseObservable.raise;

let repeat = {
  let defaultPredicate =
    fun
    | None => true
    | Some(_) => false;
  (~predicate=RxFunctions.alwaysTrue1) => {
    let predicate =
      predicate === RxFunctions.alwaysTrue1 ?
        defaultPredicate :
        (
          fun
          | None => predicate()
          | Some(_) => false
        );
    RepeatObservable.repeat(predicate);
  };
};

let retry = {
  let defaultPredicate =
    fun
    | None => false
    | Some(_) => true;

  (~predicate=RxFunctions.alwaysTrue1) => {
    let predicate =
      predicate === RxFunctions.alwaysTrue1 ?
        defaultPredicate :
        (
          fun
          | None => false
          | Some(exn) => predicate(exn)
        );

    RepeatObservable.repeat(predicate);
  };
};

let scan = (reducer, initialValue, observable) =>
  observable |> RxObservable.lift(RxOperators.scan(reducer, initialValue));

let share = observable =>
  MulticastObservable.create(RxSubject.create, observable);

let shareWithReplayBuffer = count => {
  let createSubject = () => RxSubject.createWithReplayBuffer(count);
  obs => MulticastObservable.create(createSubject, obs);
};

let some = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.some(predicate));

let startWithList = (~scheduler=?, values, observable) =>
  concat([ofList(~scheduler?, values), observable]);

let startWithValue = (~scheduler=?, value, observable) =>
  concat([ofValue(~scheduler?, value), observable]);

let subscribeOn = SubscribeOnObservable.subscribeOn;

let switch_ = observable =>
  observable |> RxObservable.lift(RxOperators.switch_);

let synchronize = observable =>
  observable |> RxObservable.lift(RxOperators.synchronize);

let timeout = (~scheduler, due, observable) =>
  observable |> RxObservable.lift(RxOperators.timeout(~scheduler, due));

let toList = {
  let toListAccumulator = (acc, next) => [next, ...acc];

  observable =>
    observable |> scan(toListAccumulator, []) |> last |> map(List.rev);
};

let withLatestFrom = (~selector, other, source) =>
  source |> RxObservable.lift(RxOperators.withLatestFrom(~selector, other));