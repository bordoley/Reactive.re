type t = {
  executor: 'a 'state .unit => SchedulerExecutor.t('state, 'a),
  now: unit => float,
};

let now = ({now}) => now();

let scheduleAfter = (~delay, work, state, {executor}) => {
  let continuation = SchedulerContinuation.create(executor(), work);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter1 = (~delay, work, state, ctx0, {executor}) => {
  let continuation = SchedulerContinuation.create1(executor(), work, ctx0);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter2 = (~delay, work, state, ctx0, ctx1, {executor}) => {
  let continuation =
    SchedulerContinuation.create2(executor(), work, ctx0, ctx1);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter3 = (~delay, work, state, ctx0, ctx1, ctx2, {executor}) => {
  let continuation =
    SchedulerContinuation.create3(executor(), work, ctx0, ctx1, ctx2);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter4 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, {executor}) => {
  let continuation =
    SchedulerContinuation.create4(executor(), work, ctx0, ctx1, ctx2, ctx3);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter5 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, {executor}) => {
  let continuation =
    SchedulerContinuation.create5(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
    );
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter6 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {executor}) => {
  let continuation =
    SchedulerContinuation.create6(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter7 =
    (
      ~delay,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      {executor},
    ) => {
  let continuation =
    SchedulerContinuation.create7(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
    );
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule = (work, state, scheduler) =>
  scheduleAfter(~delay=0.0, work, state, scheduler);

let schedule1 = (work, state, ctx0, scheduler) =>
  scheduleAfter1(~delay=0.0, work, state, ctx0, scheduler);

let schedule2 = (work, state, ctx0, ctx1, scheduler) =>
  scheduleAfter2(~delay=0.0, work, state, ctx0, ctx1, scheduler);

let schedule3 = (work, state, ctx0, ctx1, ctx2, scheduler) =>
  scheduleAfter3(~delay=0.0, work, state, ctx0, ctx1, ctx2, scheduler);

let schedule4 = (work, state, ctx0, ctx1, ctx2, ctx3, scheduler) =>
  scheduleAfter4(~delay=0.0, work, state, ctx0, ctx1, ctx2, ctx3, scheduler);

let schedule5 = (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) =>
  scheduleAfter5(
    ~delay=0.0,
    work,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    scheduler,
  );

let schedule6 = (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) =>
  scheduleAfter6(
    ~delay=0.0,
    work,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    scheduler,
  );

let schedule7 =
    (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) =>
  scheduleAfter7(
    ~delay=0.0,
    work,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    scheduler,
  );

let scheduleBy = (~time, work, state, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = state => {
      work(state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter(~delay, work, state, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy1 = (~time, work, state, ctx0, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, state) => {
      work(ctx0, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter1(~delay=0.0, work, state, ctx0, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy2 = (~time, work, state, ctx0, ctx1, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, state) => {
      work(ctx0, ctx1, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter2(~delay=0.0, work, state, ctx0, ctx1, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy3 = (~time, work, state, ctx0, ctx1, ctx2, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, state) => {
      work(ctx0, ctx1, ctx2, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter3(~delay=0.0, work, state, ctx0, ctx1, ctx2, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy4 = (~time, work, state, ctx0, ctx1, ctx2, ctx3, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, state) => {
      work(ctx0, ctx1, ctx2, ctx3, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter4(
      ~delay=0.0,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};

let scheduleBy5 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, state) => {
      work(ctx0, ctx1, ctx2, ctx3, ctx4, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter5(
      ~delay=0.0,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};

let scheduleBy6 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state) => {
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter6(
      ~delay=0.0,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};

let scheduleBy7 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state) => {
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter7(
      ~delay=0.0,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};