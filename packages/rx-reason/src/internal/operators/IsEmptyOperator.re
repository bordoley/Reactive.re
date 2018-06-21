exception CompleteWithoutErrorException;

let operator = {
  let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

  let onNext = (self, delegate, _) => {
    delegate |> Subscriber.next(false);
    self^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
  };

  let onComplete = (_, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) => None
      | Some(_) => exn
      | None =>
        delegate |> Subscriber.next(true);
        exn;
      };
    delegate |> Subscriber.complete(~exn?);
  };

  subscriber => {
    let self = ref(Subscriber.disposed);
    self := subscriber |> Subscriber.delegate1(~onNext, ~onComplete, self);
    self^;
  };
};

let lift = observable => observable |> ObservableSource.lift(operator);