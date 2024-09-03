open Belt;

let use = dependency => {
  let elemRef = React.useRef(Js.Nullable.null);
  let firstUpdate = React.useRef(true);

  React.useEffect2(
    () => {
      let scrollIntoView = () => {
        elemRef.current
        ->Js.toOption
        ->Option.forEach(elem =>
            elem->Bindings.Element.scrollIntoView({
              behavior: "auto",
              block: "nearest",
            })
          );
      };
      // Need to skip the first scroll, to prevent scrolling on receiving initial dependency
      firstUpdate.current ? firstUpdate.current = false : scrollIntoView();

      None;
    },
    (elemRef, dependency),
  );

  elemRef->ReactDOM.Ref.domRef;
};
