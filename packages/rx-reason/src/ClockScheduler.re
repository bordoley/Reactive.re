type t = (~computeDelay: float => float) => Scheduler.t;

let make = (
  ~getTime: unit => float,
  ~scheduler: DelayScheduler.t,
) : t =>
  (~computeDelay, work) => {
    let currentTime = getTime();
    let delay = computeDelay(currentTime);
    scheduler(~delay, work);
  };