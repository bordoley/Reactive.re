open Functions.Operators;
open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "Observable",
    [
      describe("combineLatest2", []),
      describe("combineLatest3", []),
      describe("combineLatest4", []),
      describe("combineLatest5", []),
      describe("combineLatest6", []),
      describe("combineLatest7", []),
      describe(
        "concat",
        [
          it("", () =>
            Observable.concat([
              Observable.ofList([1, 2, 3]),
              Observable.ofValue(4),
              Observable.ofValue(5),
              Observable.ofList([6, 7, 8]),
            ])
            |> Observable.subscribe(
                 ~onNext=Js.log,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore
          ),
        ],
      ),
      describe(
        "create",
        [
          it(
            "onNext and onComplete function calls delegate to the subscriber",
            () => {
            let observable =
              Observable.create((~onNext, ~onComplete) => {
                onNext(10);
                onNext(20);
                onComplete(None);
                Disposable.disposed;
              });

            let value = ref(0);
            let complete = ref(false);
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=next => value := next,
                   ~onComplete=_ => complete := true,
                 );

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            value^ |> Expect.toBeEqualToInt(20);
            complete^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "createWithObserver",
        [
          it(
            "the observer instance passed to the onSubscribe function is returned as the subscription",
            () => {
              let observerInstance = MutableOption.create();
              let observable =
                Observable.createWithObserver(observer => {
                  MutableOption.set(observer, observerInstance);
                  Disposable.disposed;
                });
              let subscription =
                observable
                |> Observable.subscribe(
                     ~onNext=Functions.alwaysUnit,
                     ~onComplete=Functions.alwaysUnit,
                   );
              let observer =
                observerInstance |> MutableOption.get |> Observer.toDisposable;
              subscription === observer |> Expect.toBeEqualToTrue;
            },
          ),
          it("disposing the subscription disposes the returned observable", () => {
            let returnedDisposable = Disposable.empty();
            let observable =
              Observable.createWithObserver(_ => returnedDisposable);
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );

            Disposable.dispose(subscription);
            returnedDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it(
            "onSubscribe raising an exception is caught and completes the observer",
            () => {
            let observable =
              Observable.createWithObserver(_ => raise(Division_by_zero));

            let completed = ref(None);
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=exn =>
                   completed := exn
                 );
            switch (completed^) {
            | Some(Division_by_zero) => ()
            | _ => failwith("expected exception to be thrown")
            };
          }),
          it(
            "subscribe rethrows exceptions from onSubscribe if the observer is already completed",
            () => {
              let observable =
                Observable.createWithObserver(observer => {
                  observer |> Observer.complete(None);
                  raise(Division_by_zero);
                });
              Expect.shouldRaise(() =>
                observable
                |> Observable.subscribe(
                     ~onNext=Functions.alwaysUnit,
                     ~onComplete=Functions.alwaysUnit,
                   )
              );
            },
          ),
        ],
      ),
      describe(
        "defer",
        [
          it("calls the observable factory on subscribe", () => {
            let count = ref(0);

            let observable =
              Observable.defer(() => {
                count := count^ + 1;
                Observable.empty();
              });

            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;

            observable
            |> Observable.subscribe(
                 ~onNext=Functions.alwaysUnit,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            observable
            |> Observable.subscribe(
                 ~onNext=Functions.alwaysUnit,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe("empty", []),
      describe(
        "lift",
        [
          it("applies the operator", () =>
            Observable.ofList([1, 2, 3])
            |> Observable.lift(
                 Operators.scan((acc, next) => acc + next, 0)
                 << Operators.map(string_of_int)
                 << Operators.last,
               )
            |> Observable.subscribe(
                 ~onNext=Expect.toBeEqualToString("6"),
                 ~onComplete=Functions.alwaysUnit,
               )
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue
          ),
        ],
      ),
      describe(
        "merge",
        [
          it("merges host and cold observables", () => {
            let result = ref([]);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            Observable.merge([
              subject1 |> Subject.toObservable,
              Observable.ofList([1, 2, 3]),
              subject2 |> Subject.toObservable,
              Observable.ofList([4, 5, 6]),
            ])
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer1 |> Observer.next(7);
            observer2 |> Observer.next(8);
            observer1 |> Observer.next(9);
            result^
            |> Expect.toBeEqualToListOfInt([9, 8, 7, 6, 5, 4, 3, 2, 1]);
          }),
          it("merges hot observables", () => {
            let result = ref([]);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            Observable.merge([
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
            ])
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer1 |> Observer.next(1);
            observer2 |> Observer.next(2);
            observer1 |> Observer.next(3);
            observer2 |> Observer.next(4);
            observer1 |> Observer.complete(None);
            observer2 |> Observer.next(5);
            observer2 |> Observer.complete(None);
            observer3 |> Observer.next(6);
            observer3 |> Observer.complete(None);

            result^ |> Expect.toBeEqualToListOfInt([6, 5, 4, 3, 2, 1]);
          }),
          it("completes when the last observable completes", () => {
            let result = ref(false);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let subscription =
              Observable.merge([
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=_ =>
                   result := true
                 );

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer1 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer2 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer3 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes early if any observable completes with an exception", () => {
            let result = ref(false);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let subscription =
              Observable.merge([
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=_ =>
                   result := true
                 );

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer1 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer2 |> Observer.complete(Some(Division_by_zero));
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe("never", []),
      describe("ofList", []),
      describe("ofValue", []),
      describe("retry", []),
      describe(
        "startWithlist",
        [
          it("", () => {
            let result = ref([]);
            let subject = Subject.create();
            let subscription =
              Observable.startWithList(
                [1, 2, 3],
                subject |> Subject.toObservable,
              )
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                   ~onComplete=Functions.alwaysUnit,
                 );
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);

            let observer = subject |> Subject.toObserver;
            observer |> Observer.next(4);
            observer |> Observer.next(5);
            observer |> Observer.complete(None);
            Js.log(result^);
            /*result^ |> Expect.toBeEqualToListOfInt([5, 4, 1, 2, 3]);*/
          }),
        ],
      ),
      describe("startWithValue", []),
      describe("subscribe", []),
      describe("subscribeObserver", []),
      describe("subscribeOn", []),
    ],
  );