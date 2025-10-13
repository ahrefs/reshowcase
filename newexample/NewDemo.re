
let demos: list(Reshowcase.NewEntity.t) = [
  Category({
    name: "Buttons",
    demos: [
      {
        name: "Normal",
        modulePath: ButtonNormal.modulePath,
      },
      {
        name: "Huge",
        modulePath: ButtonHuge.modulePath,
      },
    ],
  }),
  Category({
    name: "Headings",
    demos: [
      {
        name: "H1",
        modulePath: H1.modulePath,
      },
      {
        name: "H2",
        modulePath: H2.modulePath,
      },
    ],
  }),
  Category({
    name: "Text",
    demos: [
      {
        name: "Paragraph",
        modulePath: Paragraph.modulePath,
      },
      {
        name: "Italic",
        modulePath: Italic.modulePath,
      },
    ],
  }),
  Demo({
    name: "Code example",
    modulePath: CodeExample.modulePath,
  }),
  Category({
    name: "Test search",
    demos: [
      {
        name: "OneTwoThreeFour",
        modulePath: OneTwoThreeFour.modulePath,
      },
      {
        name: "OneTwoThreeFive",
        modulePath: OneTwoThreeFive.modulePath,
      },
      {
        name: "OneTwoFourSeven",
        modulePath: OneTwoFourSeven.modulePath,
      },
    ],
  }),
];

Reshowcase.NewEntry.start(~demos);
