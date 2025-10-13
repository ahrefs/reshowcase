
let demos: list(Reshowcase.NewEntity.t) = [
  Category({
    name: "Buttons",
    demos: [
      {
        name: "Normal",
        modulePath: "ButtonNormal",
      },
      {
        name: "Huge",
        modulePath: "ButtonHuge",
      },
    ],
  }),
  Category({
    name: "Headings",
    demos: [
      {
        name: "H1",
        modulePath: "H1",
      },
      {
        name: "H2",
        modulePath: "H2",
      },
    ],
  }),
  Category({
    name: "Text",
    demos: [
      {
        name: "Paragraph",
        modulePath: "Paragraph",
      },
      {
        name: "Italic",
        modulePath: "Italic",
      },
    ],
  }),
  Demo({
    name: "Code example",
    modulePath: "CodeExample",
  }),
  Category({
    name: "Test search",
    demos: [
      {
        name: "OneTwoThreeFour",
        modulePath: "OneTwoThreeFour",
      },
      {
        name: "OneTwoThreeFive",
        modulePath: "OneTwoThreeFive",
      },
      {
        name: "OneTwoFourSeven",
        modulePath: "OneTwoFourSeven",
      },
    ],
  }),
];

Reshowcase.NewEntry.start(~demos);
