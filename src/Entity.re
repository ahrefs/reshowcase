type t =
  | Demo(Configs.demoUnitProps => React.element)
  | Category(Js.Dict.t(t));
