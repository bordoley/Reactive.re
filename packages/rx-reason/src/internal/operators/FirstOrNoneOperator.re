let operator = subscriber =>
  FirstOperator.operator @@
  MapOperator.operator(Functions.some) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;