open Belt;
// open Prelude;
// open Layout;
module URLSearchParams = Bindings.URLSearchParams;
module Window = Bindings.Window;
module LocalStorage = Bindings.LocalStorage;

module App = {
  module Css = {
    open StyleVars;

    let app = [%cx
      {|
      display: flex;
      flex-direction: row;
      min-height: 100vh;
      align-items: stretch;
      color: $(Color.darkGray);
    |}
    ];

    let main = [%cx
      {|
      flex-grow: 1;
      display: flex;
      flex-direction: column;
    |}
    ];

    let empty = [%cx
      {|
      flex-grow: 1;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
    |}
    ];

    let emptyText = [%cx
      {|
      font-size: $(FontSize.lg);
      color: $(Color.black40a);
      text-align: center;
    |}
    ];

    let right = [%cx
      {|
      display: flex;
      flex-direction: column;
      width: 100%;
    |}
    ];

    let demo = [%cx
      {|
      display: flex;
      flex: 1;
      flex-direction: row;
      align-items: stretch;
    |}
    ];

    let demoContents = [%cx
      {|
      display: flex;
      flex: 1;
      flex-direction: column;
    |}
    ];
  };

  type route =
    | Demo(list(string))
    | Home;

  [@react.component]
  let make = (~items: array(NewEntity.item)) => {
    let url = ReasonReactRouter.useUrl();
    let urlSearchParams = url.search->URLSearchParams.make;
    let route =
      {switch (url.path) {
        | [] => Home
        | pathParts => Demo(pathParts)
      }}

    let (_iframeKey, setIframeKey) =
      React.useState(() => Js.Date.now()->Float.toString);

    React.useEffect1(
      () => {
        setIframeKey(_ => Js.Date.now()->Float.toString);
        None;
      },
      [|url|],
    );

    let (isCategoriesCollapsedByDefault, toggleIsCategoriesCollapsed) =
      React.useState(() =>
        switch (
          LocalStorage.localStorage->LocalStorage.getItem(
            "isCategoriesCollapsedByDefault",
          )
        ) {
        | Some("true") => true
        | _ => false
        }
      );

    let onToggleCollapsedCategoriesByDefault = () => {
      toggleIsCategoriesCollapsed(_ => !isCategoriesCollapsedByDefault);
      LocalStorage.localStorage->LocalStorage.setItem(
        "isCategoriesCollapsedByDefault",
        isCategoriesCollapsedByDefault ? "false" : "true",
      );
    };

    <div name="App" className=Css.app>
      {switch (route) {
       | Demo(_pathParts) => <div className=Css.main> "Demo"->React.string </div>
       | Home =>
         <>
           <NewDemoListSidebar
             items
             urlSearchParams
             isCategoriesCollapsedByDefault
             onToggleCollapsedCategoriesByDefault
           />
           <div className=Css.empty>
             <div className=Css.emptyText> "Pick a demo"->React.string </div>
           </div>
         </>
       }}
    </div>;
  };
};
