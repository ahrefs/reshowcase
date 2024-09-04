module Color = {
  let toString = CSS.Types.Color.toString;

  let white = `hex("fff");
  let lightGray = `hex("f5f6f6");
  let midGray = `hex("e0e2e4");
  let darkGray = `hex("42484d");
  let black40a = `rgba((0, 0, 0, `num(0.4)));
  let blue = `hex("0091ff");
  let orange = `hex("ffae4b");
};

module Gap = {
  let xxs = `px(2);
  let xs = `px(5);
  let md = `px(8);
};

module BorderRadius = {
  let default = `px(5);
};

module FontSize = {
  let sm = `px(12);
  let md = `px(14);
  let lg = `px(20);
};
