type t('a);

include Observable.S1 with type t('a) := t('a);

let share: Observable.t('a) => t('a);
let shareWithReplayBuffer: (int, Observable.t('a)) => t('a);