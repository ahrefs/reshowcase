open Reshowcase.Entry;

module Cn = {
  let ifTrue = (cn, x) => x ? cn : "";
};

let spaceConcat = (x1, x2) =>
  switch (x1, x2) {
  | ("", x)
  | (x, "") => x
  | (x1, x2) => x1 ++ " " ++ x2
  };

let (+++) = spaceConcat;

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

  let h1Size = size => {
    let fontSize = `px(size);
    [%cx {|
      font-size: $(fontSize);
    |}];
  };

  let code = [%cx
    {|
    white-space: pre;
    padding: 0;
    background-color: #f5f6f6;
  |}
  ];
};

demo(({addDemo: _, addCategory}) =>
  addCategory("Buttons", ({addDemo, addCategory: _}) => {
    addDemo("Normal", ({string, bool, _}) => {
      let disabled = bool("Disabled", false);
      let color =
        string(
          "Color",
          ~options=[|
            ("Red", "E02020"),
            ("Green", "6DD400"),
            ("Blue", "0091FF"),
          |],
          "0091FF",
        );
      <button
        disabled
        className={
          Css.button
          +++ Css.buttonDisabled->Cn.ifTrue(disabled)
          +++ Css.buttonColor(color)
        }>
        {string("Text", "hello")->React.string}
      </button>;
    });
    addDemo("Huge", ({string, bool, _}) => {
      let disabled = bool("Disabled", false);
      let color =
        string(
          "Color",
          ~options=[|
            ("Red", "E02020"),
            ("Green", "6DD400"),
            ("Blue", "0091FF"),
          |],
          "0091FF",
        );
      <button
        disabled
        className={
          Css.button
          +++ Css.buttonHuge
          +++ Css.buttonDisabled->Cn.ifTrue(disabled)
          +++ Css.buttonColor(color)
        }>
        {string("Text", "Hello")->React.string}
      </button>;
    });
  })
);

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
);

demo(({addDemo, addCategory: _}) =>
  addDemo("Code example", _propsApi =>
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
    </code>
  )
);

demo(({addDemo: _, addCategory}) =>
  addCategory("Test search", ({addDemo, addCategory: _}) => {
    addDemo("OneTwoThreeFour", _ => React.null);
    addDemo("OneTwoThreeFive", _ => React.null);
    addDemo("OneTwoFourSeven", _ => React.null);
  })
);

start();
