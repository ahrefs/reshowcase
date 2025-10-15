let items: array(Reshowcase.NewEntity.item) = [|
  Category({
    name: "Buttons",
    items: [|
      Demo({
        name: ButtonNormal_Doc.demoName,
        modulePath: ButtonNormal_Doc.modulePath,
      }),
      Demo({
        name: ButtonHuge_Doc.demoName,
        modulePath: ButtonHuge_Doc.modulePath,
      }),
    |],
  }),
  Category({
    name: "Headings",
    items: [|
      Demo({
        name: H1_Doc.demoName,
        modulePath: H1_Doc.modulePath,
      }),
      Demo({
        name: H2_Doc.demoName,
        modulePath: H2_Doc.modulePath,
      }),
    |],
  }),
  Category({
    name: "Text",
    items: [|
      Demo({
        name: Paragraph_Doc.demoName,
        modulePath: Paragraph_Doc.modulePath,
      }),
      Demo({
        name: Italic_Doc.demoName,
        modulePath: Italic_Doc.modulePath,
      }),
      Category({
        name: "Nested Text",
        items: [|
          Demo({
            name: Italic_Doc.demoName,
            modulePath: Italic_Doc.modulePath,
          }),
        |],
      }),
    |],
  }),
  Demo({
    name: CodeExample_Doc.demoName,
    modulePath: CodeExample_Doc.modulePath,
  }),
  Category({
    name: "Test search",
    items: [|
      Demo({
        name: OneTwoThreeFour_Doc.demoName,
        modulePath: OneTwoThreeFour_Doc.modulePath,
      }),
      Demo({
        name: OneTwoThreeFive_Doc.demoName,
        modulePath: OneTwoThreeFive_Doc.modulePath,
      }),
      Demo({
        name: OneTwoFourSeven_Doc.demoName,
        modulePath: OneTwoFourSeven_Doc.modulePath,
      }),
    |],
  }),
|];

let () = Reshowcase.NewEntry.start(~items, ~mode=Build, ());
