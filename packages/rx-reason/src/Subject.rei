/**
 * A type that both an Observable sequence as well as an Subscriber.
 */;
type t('a);

include Disposable.S1 with type t('a) := t('a);
include Observable.S1 with type t('a) := t('a);
include ObserverLike.S1 with type t('a) := t('a);

/** Constructs a new Subject instance. */
let create: unit => t('a);

/**
 * Constructs a new Subject instance with the specified buffer size
 * which will replay the most recent buffered notifications when
 * subscribed to.
 * */
let createWithReplayBuffer: int => t('a);

let forwardOnComplete: (t('a), option(exn)) => unit;

let forwardOnNext: (t('a), 'a) => unit;

/**
 * A disposed Subject instance.
 */
let disposed: t('a);