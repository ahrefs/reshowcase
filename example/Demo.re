open Reshowcase.Entry;

module Styles = {
  let empty = CSS.make("", []);

  let ifTrue = (styles, x) => x ? styles : empty;
};

let (+++) = CSS.merge;

module Css = {
  let button = [%css
    {|
    color: #fff;
    border: none;
    padding: 10px;
    border-radius: 10px;
    font-family: inherit;
    font-size: inherit;
  |}
  ];

  let buttonHuge = [%css {|
    padding: 20px;
    font-size: 30px;
  |}];

  let buttonDisabled = [%css {|
    cursor: default;
    opacity: 0.5;
  |}];

  let buttonColor = color => {
    let color = `hex(color);
    [%css {|
      background-color: $(color);
    |}];
  };

  let h1Size = size => {
    let fontSize = `px(size);
    [%css {|
      font-size: $(fontSize);
    |}];
  };

  let code = [%css
    {|
    white-space: pre;
    padding: 0;
    background-color: #f5f6f6;
  |}
  ];
};

demo(({ addDemo: _, addCategory }) =>
  addCategory("Buttons", ({ addDemo, addCategory: _ }) => {
    addDemo("Normal", ({ string, bool, _ }) => {
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
        styles={
          Css.button
          +++ Css.buttonDisabled->Styles.ifTrue(disabled)
          +++ Css.buttonColor(color)
        }>
        {string("Text", "hello")->React.string}
      </button>;
    });
    addDemo("Huge", ({ string, bool, _ }) => {
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
        styles={
          Css.button
          +++ Css.buttonHuge
          +++ Css.buttonDisabled->Styles.ifTrue(disabled)
          +++ Css.buttonColor(color)
        }>
        {string("Text", "Hello")->React.string}
      </button>;
    });
  })
);

demo(({ addDemo: _, addCategory }) =>
  addCategory("Typography", ({ addDemo: _, addCategory }) => {
    addCategory("Headings", ({ addDemo, addCategory: _ }) => {
      addDemo("H1", ({ string, int, _ }) => {
        let size =
          int(
            "Font size",
            {
              min: 0,
              max: 100,
              initial: 30,
              step: 1,
            },
          );

        <h1 styles={Css.h1Size(size)}>
          {string("Text", "hello")->React.string}
        </h1>;
      });
      addDemo("H2", ({ string, _ }) =>
        <h2> {string("Text", "hello")->React.string} </h2>
      );
    });
    addCategory("Text", ({ addDemo, addCategory: _ }) => {
      addDemo("Paragraph", ({ string, _ }) =>
        <p> {string("Text", "hello")->React.string} </p>
      );
      addDemo("Italic", ({ string, _ }) =>
        <i> {string("Text", "hello")->React.string} </i>
      );
    });
  })
);

demo(({ addDemo, addCategory: _ }) =>
  addDemo("Code example", _propsApi =>
    <code styles=Css.code>
      {js|open Reshowcase.Entry;

demo(({addDemo: _, addCategory}) =>
  addCategory("Typography", ({addDemo: _, addCategory}) => {
    addCategory("Headings", ({addDemo, addCategory: _}) => {
      addDemo("H1", ({string, int, _}) => {
        let size =
          int("Font size", {min: 0, max: 100, initial: 30, step: 1});

        <h1 styles={Css.h1Size(size)}>
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

demo(({ addDemo: _, addCategory }) =>
  addCategory("Test search", ({ addDemo, addCategory: _ }) => {
    addDemo("OneTwoThreeFour", _ => React.null);
    addDemo("OneTwoThreeFive", _ => React.null);
    addDemo("OneTwoFourSeven", _ => React.null);
  })
);

start();
