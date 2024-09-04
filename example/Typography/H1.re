module Component = {
  [@react.component]
  let make = () => {
    <h1
      style={ReactDOM.Style.make(
        ~color="#000",
        ~border="none",
        ~padding="10px",
        ~borderRadius="10px",
        ~fontFamily="inherit",
        ~fontSize="inherit",
        (),
      )}>
      "Hello"->React.string
    </h1>;
  };
};

switch (ReactDOM.querySelector("body")) {
| Some(node) =>
  let root = ReactDOM.Client.createRoot(node);
  ReactDOM.Client.render(root, <Component />);
| None =>
  Js.Console.error(
    "RR.renderToElementWithId : no element of id '"
    ++ "body"
    ++ "' found in the HTML.",
  )
};
