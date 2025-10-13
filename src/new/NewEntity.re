open Melange_json.Primitives;

[@deriving (json, json_string)]
type demo = {
  name: string,
  modulePath: string,
};

[@deriving (json, json_string)]
type category = {
  name: string,
  items: array(item),
}
and item =
  | Demo(demo)
  | Category(category);

[@deriving (json, json_string)]
type items = array(item);
