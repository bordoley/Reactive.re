let module StatefulComponent = RxReasonReact.Component.Make({
  type props = string;
  type state = Store.State.t;

  let name = "RxReasonReactExampleContainer";
  let createStore = Store.create; 
  let render = (~state as {count, greeting, incrementCount, show, toggle}: Store.State.t, _) =>
    <GreetingComponent count greeting incrementCount show toggle />;
});

let promise: Js.Promise.t(int) = Js.Promise.resolve(1);
let single = promise |> RxReasonJs.JSPromise.toSingle;
single |> RxReason.Single.subscribeWithCallbacks(
  ~onSuccess=Js.log,
  ~onError=Js.log,
) |> ignore;

let promise = single 
  |> RxReason.Single.toObservable 
  |> RxReason.Single.liftFirst(RxReason.Operators.mapTo("b")) 
  |> RxReasonJs.JSPromise.fromSingle
  |> Js.Promise.then_(a => { Js.log(a); Js.Promise.resolve(())});

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    ReactDOMRe.renderToElementWithId(
      <StatefulComponent props=(state^ ? "true" : "false") />, 
      "index2",
    );
    state := state^ |> (!); 
  },
  5000,
)|> ignore;