type context('a) = {
  predicate: 'a => bool,
  mutable self: Subscriber.t('a),
};

let operator = {
  let onNext = {
    let impl = ({predicate, self}, subscriber, next) => {
      let shouldKeep =
        try (predicate(next)) {
        | exn =>
          self |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };
      if (shouldKeep) {
        subscriber |> Subscriber.next(next);
      };
    };

    (ctx, subscriber, next) =>
      Functions.earlyReturnsUnit3(impl, ctx, subscriber, next);
  };

  (predicate, subscriber) => {
    let context = {predicate, self: Subscriber.disposed};

    context.self =
      subscriber
      |> Subscriber.decorate1(
           ~onNext,
           ~onComplete=Subscriber.forwardOnComplete1,
           context,
         );

    context.self;
  };
};