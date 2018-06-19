exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator =
  observer => {
    let isEmptyObserver = ref(Observer.disposed);
    isEmptyObserver :=
      Observer.delegate(
        ~onNext=
          _ => {
            observer |> Observer.next(false);
            isEmptyObserver^
            |> Observer.complete(~exn=?completeWithoutErrorExn);
          },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(CompleteWithoutErrorException) => None
              | Some(_) => exn
              | None =>
                observer |> Observer.next(true);
                exn;
              };
            observer |> Observer.complete(~exn?);
          },
        observer,
      );
    isEmptyObserver^;
  };

let lift = observable => observable |> ObservableSource.lift(operator);