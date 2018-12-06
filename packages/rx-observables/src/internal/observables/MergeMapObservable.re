let create = (~maxBufferSize=?, ~maxConcurrency=?, f, observable) =>
  observable
  |> RxObservable.lift(
       MergeMapOperator.create(~maxBufferSize?, ~maxConcurrency?, f),
     );


let create1 = (~maxBufferSize=?, ~maxConcurrency=?, f, ctx0, observable) =>
  observable
  |> RxObservable.lift(
       MergeMapOperator.create1(~maxBufferSize?, ~maxConcurrency?, f, ctx0),
     );