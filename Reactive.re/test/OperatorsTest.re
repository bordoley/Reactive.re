open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Operators",
    [
      describe(
        "identity",
        [
          it("returns the observer provided as an argument", () => {
            let observer = Observer.create();
            let identityObserver = Operators.identity(observer);
            observer === identityObserver |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "map",
        [
          it("completes the observer when the mapper throws an exception", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            let mapper = (_) => raise(Division_by_zero);
            let mappingObserver = Operators.map(mapper, observer);
            mappingObserver |> Observer.next(1);
            observedExn^ === None |> Expect.toBeEqualToFalse;
            mappingObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes the observer when the mapping observer is completed", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            let mappingObserver = Operators.map(a => a + 1, observer);
            mappingObserver |> Observer.completeWithResult(~exn=Some(Division_by_zero)) |> Expect.toBeEqualToTrue;
            observedExn^ === None |> Expect.toBeEqualToFalse;
            mappingObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "mapTo",
        [
          it("maps any input to value", () => {
            let observedNext = ref("");
            let observer =
              Observer.create(~onNext=next => observedNext := next, ());
            let mappingObserver = Operators.mapTo("a", observer);
            mappingObserver |> Observer.next(1);
            observedNext^ |> Expect.toBeEqualToString("a");
          }),
        ],
      ),
      describe(
        "doOnNext",
        [
          it("calls the side effect function on next", () => {
            let sideEffectCalled = ref(false);
            let observedNext = ref(0);
            let observer =
              Observer.create(~onNext=next => observedNext := next, ());
            let doObserver =
              Operators.doOnNext((_) => sideEffectCalled := true, observer);
            doObserver |> Observer.next(1);
            sideEffectCalled^ |> Expect.toBeEqualToTrue;
            observedNext^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe("keep", [
          it("completes the observer when the mapper throws an exception", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            let mapper = (_) => raise(Division_by_zero);
            let keepObserver = Operators.keep(mapper, observer);
            keepObserver |> Observer.next(1);
            observedExn^ === None |> Expect.toBeEqualToFalse;
            keepObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes the observer when the mapping observer is completed", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            let keepObserver = Operators.keep(_ => true, observer);
            keepObserver |> Observer.completeWithResult(~exn=Some(Division_by_zero)) |> Expect.toBeEqualToTrue;
            observedExn^ === None |> Expect.toBeEqualToFalse;
            keepObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
      ]),
      describe("first", [
        it("publishes the first observed value and disposes", () => {
          let observedValue = ref(0);
          let completed = ref(false);
          let observer = Observer.create(~onNext=next => observedValue := next,~onComplete=_=>completed := true, ());
          let firstObserver = Operators.first(observer);
          firstObserver |> Observer.next(2);
          firstObserver |> Observer.next(3);
          observedValue^ |> Expect.toBeEqualToInt(2);
          completed^ |> Expect.toBeEqualToTrue;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          firstObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
        it("passes through completed exceptions", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let firstObserver = Operators.first(observer);
          firstObserver |> Observer.complete(~exn=Some(Division_by_zero));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          firstObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue;
          };
        }),
        it("completes with exception if no values are produced", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let firstObserver = Operators.first(observer);
          firstObserver |> Observer.complete;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          firstObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Operators.EmptyException |> Expect.toBeEqualToTrue;
          };
        })
      ]),
      describe("firstOrNone", [
        it("publishes Some of the first observed value and disposes", () => {
          let observedValue = ref(None);
          let completed = ref(false);
          let observer = Observer.create(~onNext=next => observedValue := next,~onComplete=_=>completed := true, ());
          let firstOrNoneObserver = Operators.firstOrNone(observer);
          firstOrNoneObserver |> Observer.next(2);
          observedValue^ |> Expect.toBeEqualToSomeOfInt(2);
          completed^ |> Expect.toBeEqualToTrue;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          firstOrNoneObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
        it("passes through completed exceptions", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let firstOrNoneObserver = Operators.firstOrNone(observer);
          firstOrNoneObserver |> Observer.complete(~exn=Some(Division_by_zero));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          firstOrNoneObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue;
          };
        }),
        it("ignores EmptyException, publishes None, and completes normally", () => {
          let observedValue = ref(Some(1));
          let observer = Observer.create(~onNext=next => observedValue := next, ());
          let firstOrNoneObserver = Operators.firstOrNone(observer);
          firstOrNoneObserver |> Observer.complete(~exn=Some(Operators.EmptyException));
          observedValue^ |> Expect.toBeEqualToNoneOfInt;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          firstOrNoneObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
      ]),
      describe("maybeFirst", [
        it("publishes the first observed value and disposes", () => {
          let observedValue = ref(0);
          let completed = ref(false);
          let observer = Observer.create(~onNext=next => observedValue := next,~onComplete=_=>completed := true, ());
          let maybeFirstObserver = Operators.maybeFirst(observer);
          maybeFirstObserver |> Observer.next(2);
          observedValue^ |> Expect.toBeEqualToInt(2);
          completed^ |> Expect.toBeEqualToTrue;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          maybeFirstObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
        it("passes through completed exceptions", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let maybeFirstObserver = Operators.maybeFirst(observer);
          maybeFirstObserver |> Observer.complete(~exn=Some(Division_by_zero));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          maybeFirstObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue;
          };
        }),
        it("ignores EmptyException and completes normally", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let maybeFirstObserver = Operators.maybeFirst(observer);
          maybeFirstObserver |> Observer.complete(~exn=Some(Operators.EmptyException));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          maybeFirstObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          observedExn^ === None |> Expect.toBeEqualToTrue;
        }),
      ]),
      describe("last", [
        it("publishes the last observed value and disposes", () => {
          let observedValue = ref(0);
          let completed = ref(false);
          let observer = Observer.create(~onNext=next => observedValue := next,~onComplete=_=>completed := true, ());
          let lastObserver = Operators.last(observer);
          lastObserver |> Observer.next(2);
          observedValue^ |> Expect.toBeEqualToInt(0);
          completed^ |> Expect.toBeEqualToFalse;
          lastObserver |> Observer.next(3);
          observedValue^ |> Expect.toBeEqualToInt(0);
          completed^ |> Expect.toBeEqualToFalse;
          lastObserver |> Observer.complete;
          observedValue^ |> Expect.toBeEqualToInt(3);
          completed^ |> Expect.toBeEqualToTrue;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          lastObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
        it("passes through completed exceptions", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let lastObserver = Operators.last(observer);
          lastObserver |> Observer.next(2);
          lastObserver |> Observer.next(3);
          lastObserver |> Observer.complete(~exn=Some(Division_by_zero));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          lastObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue;
          };
        }),
        it("completes with exception if no values are produced", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let lastObserver = Operators.last(observer);
          lastObserver |> Observer.complete;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          lastObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Operators.EmptyException |> Expect.toBeEqualToTrue;
          };
        })
      ]),
      describe("lastOrNone", [
        it("publishes the Some of the last observed value and disposes", () => {
          let observedValue = ref(None);
          let completed = ref(false);
          let observer = Observer.create(~onNext=next => observedValue := next,~onComplete=_=>completed := true, ());
          let lastOrNoneObserver = Operators.lastOrNone(observer);
          lastOrNoneObserver |> Observer.next(3);
          observedValue^ |> Expect.toBeEqualToNoneOfInt;
          lastOrNoneObserver |> Observer.next(2);
          observedValue^ |> Expect.toBeEqualToNoneOfInt;
          lastOrNoneObserver |> Observer.complete;
          observedValue^ |> Expect.toBeEqualToSomeOfInt(2);
          completed^ |> Expect.toBeEqualToTrue;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          lastOrNoneObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
        it("passes through completed exceptions", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let lastOrNoneObserver = Operators.lastOrNone(observer);
          lastOrNoneObserver |> Observer.complete(~exn=Some(Division_by_zero));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          lastOrNoneObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue;
          };
        }),
        it("ignores EmptyException, publishes None, and completes normally", () => {
          let observedValue = ref(Some(1));
          let observer = Observer.create(~onNext=next => observedValue := next, ());
          let lastOrNoneObserver = Operators.lastOrNone(observer);
          lastOrNoneObserver |> Observer.complete(~exn=Some(Operators.EmptyException));
          observedValue^ |> Expect.toBeEqualToNoneOfInt;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          lastOrNoneObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
      ]),
      describe("maybeLast", [
        it("publishes the first observed value and disposes", () => {
          let observedValue = ref(0);
          let completed = ref(false);
          let observer = Observer.create(~onNext=next => observedValue := next,~onComplete=_=>completed := true, ());
          let maybeLastObserver = Operators.maybeLast(observer);
          maybeLastObserver |> Observer.next(2);
          observedValue^ |> Expect.toBeEqualToInt(0);
          maybeLastObserver |> Observer.next(4);
          observedValue^ |> Expect.toBeEqualToInt(0);
          maybeLastObserver |> Observer.next(6);
          maybeLastObserver |> Observer.complete;
          observedValue^ |> Expect.toBeEqualToInt(6);
          completed^ |> Expect.toBeEqualToTrue;
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          maybeLastObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
        }),
        it("passes through completed exceptions", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let maybeLastObserver = Operators.maybeLast(observer);
          maybeLastObserver |> Observer.complete(~exn=Some(Division_by_zero));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          maybeLastObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          switch(observedExn^){
          | None => failwith("expected observedExn to be not be None");
          | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue;
          };
        }),
        it("ignores EmptyException and completes normally", () => {
          let observedExn = ref(None);
          let observer = Observer.create(~onComplete=exn=>observedExn := exn, ());
          let maybeLastObserver = Operators.maybeLast(observer);
          maybeLastObserver |> Observer.complete(~exn=Some(Operators.EmptyException));
          observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          maybeLastObserver |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          observedExn^ === None |> Expect.toBeEqualToTrue;
        }),
      ]),
      describe("scan", [
        it("publishes all intermediate values, including the initial accumulator value", () => {
          let result = ref([]);
          let observer = Observer.create(~onNext=next => result := [next, ...(result^)], ());
          let scanObserver = Operators.scan((acc, next) => acc + next, 0, observer);
          scanObserver |> Observer.next(2);
          scanObserver |> Observer.next(3);
          scanObserver |> Observer.next(4);

          result^ |> Expect.toBeEqualToListOfInt([9,5, 2, 0]);
        }),
      ]),
      describe("distinctUntilChanged", [
        it("removes duplicates", () => {
          let result = ref([]);
          let observer = Observer.create(~onNext=next => result := [next, ...(result^)], ());
          let distinctObserver = Operators.distinctUntilChanged(observer);
          distinctObserver |> Observer.next(1);
          distinctObserver |> Observer.next(1);
          distinctObserver |> Observer.next(1);
          distinctObserver |> Observer.next(3);
          distinctObserver |> Observer.next(5);
          distinctObserver |> Observer.next(3);
          distinctObserver |> Observer.next(3);
          distinctObserver |> Observer.next(1);

          result^ |> Expect.toBeEqualToListOfInt([1, 3, 5, 3, 1]);
        }),
      ]),
      describe("switch_", []),
      describe("switchMap", []),
      describe("debounceTime", []),
    ],
  );