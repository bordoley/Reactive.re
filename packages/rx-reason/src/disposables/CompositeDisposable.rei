type t;

type compositeDisposable = t;

module type S = {
  type t;

  include Disposable.S with type t := t;

  let addCompositeDisposable: (compositeDisposable, t) => t;

  let addDisposable: (Disposable.t, t) => t;

  let addSerialDisposable: (SerialDisposable.t, t) => t;

  let addTeardown: (TeardownLogic.t, t) => t;

  let addTeardown1: (TeardownLogic.t1('b), 'b, t) => t;

  let addTeardown2: (TeardownLogic.t2('b, 'c), 'b, 'c, t) => t;

  let addTeardown3: (TeardownLogic.t3('b, 'c, 'd), 'b, 'c, 'd, t) => t;

  let addTeardown4:
    (TeardownLogic.t4('b, 'c, 'd, 'e), 'b, 'c, 'd, 'e, t) => t;

  let addTeardown5:
    (TeardownLogic.t5('b, 'c, 'd, 'e, 'f), 'b, 'c, 'd, 'e, 'f, t) => t;

  let addTeardown6:
    (TeardownLogic.t6('b, 'c, 'd, 'e, 'f, 'g), 'b, 'c, 'd, 'e, 'f, 'g, t) => t;

  let addTeardown7:
    (
      TeardownLogic.t7('b, 'c, 'd, 'e, 'f, 'g, 'h),
      'b,
      'c,
      'd,
      'e,
      'f,
      'g,
      'h,
      t
    ) =>
    t;

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let addCompositeDisposable: (compositeDisposable, t('a)) => t('a);

  let addDisposable: (Disposable.t, t('a)) => t('a);

  let addSerialDisposable: (SerialDisposable.t, t('a)) => t('a);

  let addTeardown: (TeardownLogic.t, t('a)) => t('a);

  let addTeardown1: (TeardownLogic.t1('b), 'b, t('a)) => t('a);

  let addTeardown2: (TeardownLogic.t2('b, 'c), 'b, 'c, t('a)) => t('a);

  let addTeardown3:
    (TeardownLogic.t3('b, 'c, 'd), 'b, 'c, 'd, t('a)) => t('a);

  let addTeardown4:
    (TeardownLogic.t4('b, 'c, 'd, 'e), 'b, 'c, 'd, 'e, t('a)) => t('a);

  let addTeardown5:
    (TeardownLogic.t5('b, 'c, 'd, 'e, 'f), 'b, 'c, 'd, 'e, 'f, t('a)) =>
    t('a);

  let addTeardown6:
    (
      TeardownLogic.t6('b, 'c, 'd, 'e, 'f, 'g),
      'b,
      'c,
      'd,
      'e,
      'f,
      'g,
      t('a)
    ) =>
    t('a);

  let addTeardown7:
    (
      TeardownLogic.t7('b, 'c, 'd, 'e, 'f, 'g, 'h),
      'b,
      'c,
      'd,
      'e,
      'f,
      'g,
      'h,
      t('a)
    ) =>
    t('a);

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t('a) => compositeDisposable;
};

include S with type t := t;

/** Constructs a new CompositeDisposable instance. */
let create: unit => t;

let disposed: t;