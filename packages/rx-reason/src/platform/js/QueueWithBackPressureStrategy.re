type t('a) = {
  queue: array('a),
  backPressureStrategy: BackPressureStrategy.t,
  maxSize: int,
};

let create = (~backPressureStrategy, ~maxSize) : t('a) => {
  queue: [||],
  backPressureStrategy,
  maxSize,
};

let clear = ({queue}) =>
  queue |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue));

let tryDeque = ({queue}) => queue |> Js.Array.shift;

let enqueue = (v, {queue, backPressureStrategy, maxSize} as this) => {
  let shouldApplyBackPressure = maxSize > 0 && Js.Array.length(queue) === maxSize;
  switch (backPressureStrategy) {
  | Throw when shouldApplyBackPressure =>
    failwith("BackPressureException")
  | DropOldest when shouldApplyBackPressure =>
    tryDeque(this) |> ignore;
    queue |> Js.Array.push(v) |> ignore;
  | DropLatest when shouldApplyBackPressure => ()
  | _ => queue |> Js.Array.push(v) |> ignore
  };
};