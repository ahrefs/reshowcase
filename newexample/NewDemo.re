let demos: list(Reshowcase.NewEntity.t) = [
  Category({
    name: "Buttons",
    demos: [
      {
        name: ButtonNormal.demoName,
        modulePath: ButtonNormal.modulePath,
      },
      {
        name: ButtonHuge.demoName,
        modulePath: ButtonHuge.modulePath,
      },
    ],
  }),
  Category({
    name: "Headings",
    demos: [
      {
        name: H1.demoName,
        modulePath: H1.modulePath,
      },
      {
        name: H2.demoName,
        modulePath: H2.modulePath,
      },
    ],
  }),
  Category({
    name: "Text",
    demos: [
      {
        name: Paragraph.demoName,
        modulePath: Paragraph.modulePath,
      },
      {
        name: Italic.demoName,
        modulePath: Italic.modulePath,
      },
    ],
  }),
  Demo({
    name: CodeExample.demoName,
    modulePath: CodeExample.modulePath,
  }),
  Category({
    name: "Test search",
    demos: [
      {
        name: OneTwoThreeFour.demoName,
        modulePath: OneTwoThreeFour.modulePath,
      },
      {
        name: OneTwoThreeFive.demoName,
        modulePath: OneTwoThreeFive.modulePath,
      },
      {
        name: OneTwoFourSeven.demoName,
        modulePath: OneTwoFourSeven.modulePath,
      },
    ],
  }),
];

Reshowcase.NewEntry.start(~demos);
