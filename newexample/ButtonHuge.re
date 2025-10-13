let spaceConcat = (x1, x2) =>
  switch (x1, x2) {
  | ("", x)
  | (x, "") => x
  | (x1, x2) => x1 ++ " " ++ x2
  };

let (+++) = spaceConcat;

module Cn = {
  let ifTrue = (cn, x) => x ? cn : "";
};

module Css = {
  let button = [%cx
    {|
    color: #fff;
    border: none;
    padding: 10px;
    border-radius: 10px;
    font-family: inherit;
    font-size: inherit;
  |}
  ];

  let buttonHuge = [%cx {|
    padding: 20px;
    font-size: 30px;
  |}];

  let buttonDisabled = [%cx {|
    cursor: default;
    opacity: 0.5;
  |}];

  let buttonColor = color => {
    let color = `hex(color);
    [%cx {|
      background-color: $(color);
    |}];
  };
};

[@react.component]
let make = () => {
  let disabled = false;
  let color = "0091FF";

  <button
    disabled
    className={
      Css.button
      +++ Css.buttonHuge
      +++ Css.buttonDisabled->Cn.ifTrue(disabled)
      +++ Css.buttonColor(color)
    }>
    {React.string("Hello")}
  </button>;
};
