let start = (~demos: array(NewEntity.t)) => {
  let str = Util.inspect(demos);
  Js.log2("!!! demos:\n", str);
};
