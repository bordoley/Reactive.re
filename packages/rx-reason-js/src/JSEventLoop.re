let schedule: RxReason.Scheduler.t =
  (work) => {
    let disposable = RxReason.AssignableDisposable.create();
    let run = () => {
      disposable
      |> RxReason.AssignableDisposable.set(RxReason.Disposable.disposed);
      let isDisposed =
        disposable |> RxReason.AssignableDisposable.isDisposed;
      if (! isDisposed) {
        disposable |> RxReason.AssignableDisposable.set(work());
      };
      Js.Promise.resolve(());
    };
    Js.Promise.(resolve(()) |> then_(run)) |> ignore;
    disposable |> RxReason.AssignableDisposable.toDisposable;
  };

let scheduleFuture: RxReason.DelayScheduler.t =
  (~delay, work) => {
    let disposable = RxReason.AssignableDisposable.create();
    let timeoutId =
      Js.Global.setTimeout(
        () => {
          disposable
          |> RxReason.AssignableDisposable.set(RxReason.Disposable.disposed);
          let isDisposed =
            disposable |> RxReason.AssignableDisposable.isDisposed;
          if (! isDisposed) {
            disposable |> RxReason.AssignableDisposable.set(work());
          };
        },
        int_of_float(delay),
      );
    let timeoutDisposable =
      RxReason.Disposable.create(() => Js.Global.clearTimeout(timeoutId));
    disposable |> RxReason.AssignableDisposable.set(timeoutDisposable);
    disposable |> RxReason.AssignableDisposable.toDisposable;
  };