module State = {
  type t = {
    count: int,
    greeting: string,
    show: bool,
  };
};

module Action = {
  type t =
    | Click
    | SetGreeting(string)
    | Toggle;
};

let greetingStateComponent = {
  let useMemoCb =
    BsReact.useMemo3((dispatcher, dispatch, action, ()) =>
      dispatcher |> dispatch(action)
    );

  let name = "GreetingStateComponent";

  let render = (~key as _=?, ~props, ()) => {
    let {
      BsRxReactStateDispatchComponent.RenderProps.state: {
        State.count,
        greeting,
        show,
      },
      dispatch,
      dispatcher,
    } = props;

    let incrementCount = useMemoCb(dispatcher, dispatch, Action.Click);
    let toggle = useMemoCb(dispatcher, dispatch, Action.Toggle);

    Components.greeting(
      ~props={count, greeting, show, incrementCount, toggle},
      (),
    );
  };

  BsRxReactStateDispatchComponent.create(~name, ~render, ());
};

let dispatcher: RxEvent.t(Action.t) = RxEvent.create();

let stateStore =
  RxValue.create(
    {count: 0, greeting: "Initial Greeting", show: false}: State.t,
  );

let sideEffectsSubscription = {
  let reducer = (action, state) =>
    switch (action) {
    | Action.Click => {...state, State.count: state.State.count + 1}
    | Action.SetGreeting(greeting) => {...state, State.greeting}
    | Action.Toggle => {...state, State.show: !state.State.show}
    };

  let onNext = (stateStore, action) =>
    stateStore |> RxValue.update1(reducer, action);

  let onComplete = (stateStore, _) => stateStore |> RxValue.dispose;

  dispatcher
  |> RxEvent.asObservable
  |> RxObservables.observe1(~onNext, ~onComplete, stateStore)
  |> RxObservable.connect;
};

BsReactDom.renderToElementWithId(
  greetingStateComponent(
    ~props={
      dispatch: RxEvent.dispatch,
      stateStream: stateStore |> RxValue.asObservable,
      dispatcher,
    },
    (),
  ),
  "index2",
);

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    state := ! state^;
    dispatcher
    |> RxEvent.dispatch(Action.SetGreeting(state^ ? "true" : "false"));
  },
  5000,
)
|> ignore;

BsRxDomLocation.instance
|> BsRxDomLocation.asObservable
|> RxObservables.onNext(Js.log)
|> RxObservable.connect;

BsRxDomLocation.instance |> BsRxDomLocation.update(_ => "?next=rock");

let promise: Js.Promise.t(int) = Js.Promise.resolve(1);
let observable = promise |> BsRxPromise.toObservable;
observable
|> RxObservables.onNext(Js.log)
|> RxObservables.onComplete(Js.log)
|> RxObservable.connect;

let promise =
  observable
  |> RxObservables.mapTo("b")
  |> BsRxPromise.fromObservable
  |> Js.Promise.then_(a => {
       Js.log(a);
       Js.Promise.resolve();
     });