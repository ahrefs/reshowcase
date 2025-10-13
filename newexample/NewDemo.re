let items: array(Reshowcase.NewEntity.item) = [|
  Category({
    name: "Buttons",
    items: [|
      Demo({
        name: ButtonNormal.demoName,
        modulePath: ButtonNormal.modulePath,
      }),
      Demo({
        name: ButtonHuge.demoName,
        modulePath: ButtonHuge.modulePath,
      }),
    |],
  }),
  Category({
    name: "Headings",
    items: [|
      Demo({
        name: H1.demoName,
        modulePath: H1.modulePath,
      }),
      Demo({
        name: H2.demoName,
        modulePath: H2.modulePath,
      }),
    |],
  }),
  Category({
    name: "Text",
    items: [|
      Demo({
        name: Paragraph.demoName,
        modulePath: Paragraph.modulePath,
      }),
      Demo({
        name: Italic.demoName,
        modulePath: Italic.modulePath,
      }),
    |],
  }),
  Demo({
    name: CodeExample.demoName,
    modulePath: CodeExample.modulePath,
  }),
  Category({
    name: "Test search",
    items: [|
      Demo({
        name: OneTwoThreeFour.demoName,
        modulePath: OneTwoThreeFour.modulePath,
      }),
      Demo({
        name: OneTwoThreeFive.demoName,
        modulePath: OneTwoThreeFive.modulePath,
      }),
      Demo({
        name: OneTwoFourSeven.demoName,
        modulePath: OneTwoFourSeven.modulePath,
      }),
    |],
  }),
|];

Reshowcase.NewEntry.start(~items);
