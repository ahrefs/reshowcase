open Belt;

let use = () => {
  let elemRef = React.useRef(Js.Nullable.null);

  React.useEffect1(
    () => {
      elemRef.current
      ->Js.toOption
      ->Option.forEach(elem =>
          elem->Bindings.Element.scrollIntoView({
            behavior: "auto",
            block: "nearest",
          })
        );

      None;
    },
    [|elemRef|],
  );

  elemRef->ReactDOM.Ref.domRef;
};
