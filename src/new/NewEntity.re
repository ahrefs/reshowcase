type demo = {
  name: string,
  modulePath: string,
};

type category = {
  name: string,
  demos: array(demo),
};

type t =
  | Demo(demo)
  | Category(category);
