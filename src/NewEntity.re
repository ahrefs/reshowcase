type demo = {
  name: string,
  modulePath: string,
};

type category = {
  name: string,
  demos: list(demo),
};

type t =
  | Demo(demo)
  | Category(category);
