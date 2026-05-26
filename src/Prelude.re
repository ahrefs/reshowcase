module Styles = {
  let empty = CSS.make("", []);

  let ifTrue = (styles, x) => x ? styles : empty;
};

let (+++) = CSS.merge;
