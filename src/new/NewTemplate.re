switch (ReactDOM.querySelector("#root")) {
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <NewExample />);
| None => ()
};
