module Css = {
  let h1Size = size => {
    let fontSize = `px(size);
    [%cx {|
      font-size: $(fontSize);
    |}];
  };
};

[@react.component]
let make = () => {
  let size = 30;

  <h1 className={Css.h1Size(size)}> {React.string("hello")} </h1>;
};
