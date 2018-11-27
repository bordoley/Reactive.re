let greeting = Components.greeting;

let promise: Js.Promise.t(int) = Js.Promise.resolve(1);
let observable = promise |> RxPromise.toObservable;
observable
|> RxObservables.pipe2(
     RxOperators.onNext(Js.log),
     RxOperators.onComplete(Js.log),
   )
|> ignore;

let promise =
  observable
  |> RxObservable.lift(RxOperators.mapTo("b"))
  |> RxPromise.fromObservable
  |> Js.Promise.then_(a => {
       Js.log(a);
       Js.Promise.resolve();
     });

let greetingStateComponent =
  RxReact.createElement(
    ~name="GreetingStateComponent",
    ~createStateStream=Store.create,
    greeting,
  );

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    ReactDom.renderToElementWithId(
      greetingStateComponent(~props=state^ ? "true" : "false", ()),
      "index2",
    );
    state := state^ |> (!);
  },
  5000,
)
|> ignore;