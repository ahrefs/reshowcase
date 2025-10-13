let start = (~items: array(NewEntity.item)) => {
  let str = Util.inspect(items);
  Js.log2("!!! demos:\n", str);
};
