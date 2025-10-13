module Css = {
  let code = [%cx
    {|
    white-space: pre;
    padding: 0;
    background-color: #f5f6f6;
  |}
  ];
};

[@react.component]
let make = () => {
  <code className=Css.code>
    {js|open Reshowcase.Entry;

demo(({addDemo: _, addCategory}) =>
  addCategory("Typography", ({addDemo: _, addCategory}) => {
    addCategory("Headings", ({addDemo, addCategory: _}) => {
      addDemo("H1", ({string, int, _}) => {
        let size =
          int("Font size", {min: 0, max: 100, initial: 30, step: 1});

        <h1 className={Css.h1Size(size)}>
          {string("Text", "hello")->React.string}
        </h1>;
      });
      addDemo("H2", ({string, _}) =>
        <h2> {string("Text", "hello")->React.string} </h2>
      );
    });
    addCategory("Text", ({addDemo, addCategory: _}) => {
      addDemo("Paragraph", ({string, _}) =>
        <p> {string("Text", "hello")->React.string} </p>
      );
      addDemo("Italic", ({string, _}) =>
        <i> {string("Text", "hello")->React.string} </i>
      );
    });
  })
);|js}
    ->React.string
  </code>;
};
