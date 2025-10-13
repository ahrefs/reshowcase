type demo = {
  name: string,
  modulePath: string,
};

type category = {
  name: string,
  items: array(item),
}
and item =
  | Demo(demo)
  | Category(category);
