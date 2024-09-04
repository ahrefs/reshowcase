open Reshowcase.Entry;

// - we compile every file as separate bundle
// - reshowcase looks at folder structure
// - picks up each file and creates category-demo

demo(({addDemo: _, addCategory}) =>
  addCategory("Buttons", ({addDemo, addCategory: _}) => {
    addDemo("Buttons/Normal", _ => React.null);
    addDemo("Buttons/Huge", _ => React.null);
  })
);

demo(({addDemo: _, addCategory}) =>
  addCategory("Typography", ({addDemo: _, addCategory}) => {
    addCategory("Headings", ({addDemo, addCategory: _}) => {
      addDemo("H1", _ => React.null);
      addDemo("H2", _ => React.null);
    })
  })
);

start();
