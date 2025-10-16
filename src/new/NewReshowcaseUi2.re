// open Belt;
open Prelude;
open Layout;
module URLSearchParams = Bindings.URLSearchParams;
module Window = Bindings.Window;
module LocalStorage = Bindings.LocalStorage;

type responsiveMode =
  | Mobile
  | Desktop;

module TopPanel = {
  module Css = {
    open StyleVars;

    let panel = [%cx
      {|
      display: flex;
      justify-content: flex-end;
      border-bottom: 1px solid $(Color.midGray);
    |}
    ];

    let buttonGroup = [%cx
      {|
      overflow: hidden;
      display: flex;
      flex-direction: row;
      align-items: stretch;
      border-radius: $(BorderRadius.default);
    |}
    ];

    let button = [%cx
      {|
      height: 32px;
      width: 48px;
      cursor: pointer;
      font-size: $(FontSize.sm);
      background-color: $(Color.lightGray);
      color: $(Color.darkGray);
      border: none;
      margin: 0;
      padding: 0;
      display: flex;
      align-items: center;
      justify-content: center;
    |}
    ];

    let buttonSquare = [%cx {|
      width: 32px;
    |}];

    let buttonActive = [%cx
      {|
      background-color: $(Color.blue);
      color: $(Color.white);
    |}
    ];

    let middleSection = [%cx
      {|
      display: flex;
      flex: 1;
      justify-content: center;
    |}
    ];

    let rightSection = [%cx {|
      display: flex;
    |}];
  };

  [@react.component]
  let make =
      (
        ~responsiveMode: responsiveMode,
        ~onSetResponsiveMode: (responsiveMode => responsiveMode) => unit,
      ) =>
    <div className=Css.panel>
      <div className=Css.rightSection />
      <div className=Css.middleSection>
        <PaddedBox gap=Md>
          <div className=Css.buttonGroup>
            <button
              title="Show in desktop mode"
              className={
                Css.button
                +++ Css.buttonActive->Cn.ifTrue(responsiveMode == Desktop)
              }
              onClick={event => {
                event->React.Event.Mouse.preventDefault;
                onSetResponsiveMode(_ => Desktop);
              }}>
              Icon.desktop
            </button>
            <button
              title="Show in mobile mode"
              className={
                Css.button
                +++ Css.buttonActive->Cn.ifTrue(responsiveMode == Mobile)
              }
              onClick={event => {
                event->React.Event.Mouse.preventDefault;
                onSetResponsiveMode(_ => Mobile);
              }}>
              Icon.mobile
            </button>
          </div>
        </PaddedBox>
      </div>
      <div className=Css.rightSection />
    </div>;
};

module DemoUnitFrame = {
  module Css = {
    open StyleVars;

    let container = [%cx
      {|
      flex: 1;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 1px;
      overflow-y: auto;
  |}
    ];

    let containerBackground = responsiveMode => {
      let backgroundColor =
        switch (responsiveMode) {
        | Mobile => Color.midGray
        | Desktop => Color.white
        };
      [%cx {|
        background-color: $(backgroundColor);
      |}];
    };

    let iframe = responsiveMode => {
      let height =
        switch (responsiveMode) {
        | Mobile => `px(667)
        | Desktop => `percent(100.)
        };
      let width =
        switch (responsiveMode) {
        | Mobile => `px(375)
        | Desktop => `percent(100.)
        };
      [%cx
       {|
        border: none;
        height: $(height);
        width: $(width);
      |}
      ];
    };
  };

  let useFullframeUrl: bool = [%mel.raw
    {js|typeof USE_FULL_IFRAME_URL === "boolean" ? USE_FULL_IFRAME_URL : false|js}
  ];

  [@react.component]
  let make = (~path: string, ~responsiveMode, ~onLoad: Js.t('a) => unit) => {
    <div
      name="DemoUnitFrame"
      className={Css.container +++ Css.containerBackground(responsiveMode)}>
      <iframe
        className={Css.iframe(responsiveMode)}
        src=path
        onLoad={event => {
          let iframe = event->React.Event.Synthetic.target;
          let window = iframe##contentWindow;
          onLoad(window);
        }}
      />
    </div>;
  };
};

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

module App = {
  [@react.component]
  let make = (~itemsJsonString, ~publicPath: string) => {
    let items = itemsJsonString->NewEntity.items_of_json_string;
    let url = ReasonReactRouter.useUrl();
    let route = {
      switch (url.path) {
      | [] => Home
      | pathParts => Demo(pathParts)
      };
    };

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

    let (responsiveMode, onSetResponsiveMode) = React.useState(() => Desktop);

    <div name="App" className=Css.app>
      <>
        <NewDemoListSidebar
          items
          url
          isCategoriesCollapsedByDefault
          onToggleCollapsedCategoriesByDefault
          publicPath
        />
        {switch (route) {
         | Home =>
           <div className=Css.empty>
             <div className=Css.emptyText> "Pick a demo"->React.string </div>
           </div>
         | Demo(pathParts) =>
           let publicPathSegments =
             publicPath
             ->Js.String.split(~sep="/", _)
             ->Belt.Array.keep(segment => segment != "");

           let fullPathSegments =
             Belt.Array.concat(
               publicPathSegments,
               Belt.List.toArray(pathParts),
             );

           let iframePathSegments =
             Belt.Array.concat(fullPathSegments, [|"iframe", "index.html"|]);

           let iframePath =
             "/" ++ Js.Array.join(~sep="/", iframePathSegments);

           <div name="Content" className=Css.right>
             <TopPanel responsiveMode onSetResponsiveMode />
             <div name="Demo" className=Css.demo>
               <div className=Css.demoContents>
                 <DemoUnitFrame
                   key=iframePath
                   path=iframePath
                   responsiveMode
                   onLoad={_iframeWindow => ()}
                 />
               </div>
             </div>
           </div>;
         }}
      </>
    </div>;
  };
};

[@react.component]
let make = (~itemsJsonString, ~publicPath) =>
  <ReasonReactErrorBoundary
    fallback={error => {
      Js.log(error);
      <h1> {React.string("Something went wrong")} </h1>;
    }}>
    <App itemsJsonString publicPath />
  </ReasonReactErrorBoundary>;

let modulePath = Utils.getFilepath();
