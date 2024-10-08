open Belt;
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

    let sidebarIcon = [%cx
      {|
      transition: 200ms ease-in-out transform;
    |}
    ];

    let sidebarIconActive = [%cx {|
      transform: rotate(180deg)
    |}];
  };

  [@react.component]
  let make =
      (
        ~isSidebarHidden: bool,
        ~responsiveMode: responsiveMode,
        ~onRightSidebarToggle: unit => unit,
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
      <div className=Css.rightSection>
        <PaddedBox gap=Md>
          <div className=Css.buttonGroup>
            <button
              title={isSidebarHidden ? "Show sidebar" : "Hide sidebar"}
              className={Css.button +++ Css.buttonSquare}
              onClick={event => {
                event->React.Event.Mouse.preventDefault;
                onRightSidebarToggle();
              }}>
              <div
                className={
                  Css.sidebarIcon
                  +++ Css.sidebarIconActive->Cn.ifTrue(!isSidebarHidden)
                }>
                Icon.sidebar
              </div>
            </button>
          </div>
        </PaddedBox>
      </div>
    </div>;
};

let rightSidebarId = "rightSidebar";

module SidebarLink = {
  module Css = {
    open StyleVars;

    let link = [%cx
      {|
      text-decoration: none;
      color: $(Color.blue);
      display: block;
      padding: $(Gap.xs) $(Gap.md);
      border-radius: $(BorderRadius.default);
      font-size: $(FontSize.md);
      font-weight: 500;
    |}
    ];

    let linkActive = [%cx {|
      background-color: $(Color.midGray);
    |}];
  };

  [@react.component]
  let make = (~activeDomRef=?, ~href, ~text: React.element) => {
    let url = ReasonReactRouter.useUrl();
    let path = String.concat("/", url.path);
    let isActive =
      Js.String.endsWith(~suffix=href, path ++ "?" ++ url.search);

    <a
      ref=?{isActive ? activeDomRef : None}
      href
      className={Css.link +++ Css.linkActive->Cn.ifTrue(isActive)}
      onClick={event =>
        switch (
          React.Event.Mouse.metaKey(event),
          React.Event.Mouse.ctrlKey(event),
        ) {
        | (false, false) =>
          React.Event.Mouse.preventDefault(event);
          ReasonReactRouter.push(href);
        | _ => ()
        }
      }>
      text
    </a>;
  };
};

module DemoListSidebar = {
  module Css = {
    open StyleVars;

    let categoryName = [%cx
      {|
      padding: $(Gap.xs) $(Gap.xxs);
      font-size: $(FontSize.md);
      font-weight: 500;
    |}
    ];

    let sidebarPanelWrapper = [%cx
      {|
      position: sticky;
      top: 0;
      background-color: $(Color.lightGray);
    |}
    ];

    let sidebarPanel = [%cx
      {|
      display: flex;
      align-items: center;
      gap: $(Gap.xs);
    |}
    ];

    let collapseButton = [%cx
      {|
      height: 32px;
      min-width: 32px;
      width: 32px;
      cursor: pointer;
      font-size: $(FontSize.sm);
      background-color: $(Color.white);
      color: $(Color.darkGray);
      border: 1px solid $(Color.midGray);
      border-radius: $(BorderRadius.default);
      margin: 0;
      padding: 0;
      display: flex;
      align-items: center;
      justify-content: center;
    |}
    ];
  };

  module SearchInput = {
    module Css = {
      open StyleVars;

      let inputWrapper = [%cx
        {|
        position: relative;
        display: flex;
        align-items: center;
        background-color: $(Color.midGray);
        border-radius: $(BorderRadius.default);
      |}
      ];

      let input = [%cx
        {|
        padding: $(Gap.xs) $(Gap.md);
        width: 100%;
        margin: 0;
        height: 32px;
        box-sizing: border-box;
        font-family: inherit;
        font-size: $(FontSize.md);
        border: none;
        background-color: transparent;
        border-radius: $(BorderRadius.default);
      |}
      ];

      let clearButton = [%cx
        {|
        position: absolute;
        right: 7px;
        display: flex;
        cursor: pointer;
        border: none;
        padding: 0;
        margin: 0;
        background-color: transparent;
        top: 50%;
        transform: translateY(-50%);
      |}
      ];
    };

    [@react.component]
    let make = (~autoFocus=?, ~value, ~onChange, ~onClear) =>
      <div className=Css.inputWrapper>
        <input
          ?autoFocus
          className=Css.input
          placeholder="Filter"
          value
          onChange
        />
        {value == ""
           ? React.null
           : <button className=Css.clearButton onClick={_event => onClear()}>
               Icon.close
             </button>}
      </div>;
  };

  let renderMenu =
      (
        ~isCategoriesCollapsedByDefault: bool,
        ~urlSearchParams: URLSearchParams.t,
        ~searchString,
        demos: Demos.t,
      ) => {
    let activeElementRef = UseScrollIntoView.use();

    let rec renderMenu =
            (
              ~parentCategoryMatchedSearch: bool,
              ~nestingLevel,
              ~categoryQuery,
              demos: Demos.t,
            ) => {
      let demos = demos->Js.Dict.entries;
      demos
      ->Array.map(((entityName, entity)) => {
          let searchMatchingTerms =
            HighlightTerms.getMatchingTerms(~searchString, ~entityName);

          let isEntityNameMatchSearch =
            searchString == "" || searchMatchingTerms->Belt.Array.size > 0;

          switch (entity) {
          | Entity.Demo(_) =>
            if (isEntityNameMatchSearch || parentCategoryMatchedSearch) {
              <SidebarLink
                activeDomRef=activeElementRef
                key=entityName
                href={
                  ("?demo=" ++ entityName->Js.Global.encodeURIComponent)
                  ++ categoryQuery
                }
                text={
                  <HighlightTerms text=entityName terms=searchMatchingTerms />
                }
              />;
            } else {
              React.null;
            }
          | Category(demos) =>
            if ((
                  isEntityNameMatchSearch
                  || Demos.isNestedEntityMatchSearch(demos, searchString)
                )
                || parentCategoryMatchedSearch) {
              let levelStr = Int.toString(nestingLevel);
              let categoryQueryKey = {js|category|js} ++ levelStr;
              let isCategoryInQuery =
                switch (
                  urlSearchParams->URLSearchParams.get(categoryQueryKey)
                ) {
                | Some(value)
                    when value->Js.Global.decodeURIComponent == entityName =>
                  true
                | Some(_)
                | None => false
                };

              <PaddedBox key=entityName padding=LeftRight>
                <Collapsible
                  title={
                    <div className=Css.categoryName>
                      <HighlightTerms
                        text=entityName
                        terms=searchMatchingTerms
                      />
                    </div>
                  }
                  isDefaultOpen={
                    isCategoryInQuery || !isCategoriesCollapsedByDefault
                  }
                  isForceOpen={searchString != ""}>
                  <PaddedBox padding=LeftRight>
                    {renderMenu(
                       ~parentCategoryMatchedSearch=
                         isEntityNameMatchSearch || parentCategoryMatchedSearch,
                       ~nestingLevel=nestingLevel + 1,
                       ~categoryQuery=
                         (
                           (({js|&category|js} ++ levelStr) ++ {js|=|js})
                           ++ entityName->Js.Global.encodeURIComponent
                         )
                         ++ categoryQuery,
                       demos,
                     )}
                  </PaddedBox>
                </Collapsible>
              </PaddedBox>;
            } else {
              React.null;
            }
          };
        })
      ->React.array;
    };

    renderMenu(
      ~parentCategoryMatchedSearch=false,
      ~nestingLevel=0,
      ~categoryQuery="",
      demos: Demos.t,
    );
  };

  [@react.component]
  let make =
      (
        ~urlSearchParams: URLSearchParams.t,
        ~demos: Demos.t,
        ~isCategoriesCollapsedByDefault: bool,
        ~onToggleCollapsedCategoriesByDefault: unit => unit,
      ) => {
    let (filterValue, setFilterValue) = React.useState(() => None);
    <Sidebar fullHeight=true>
      <div className=Css.sidebarPanelWrapper>
        <PaddedBox gap=Md border=Bottom>
          <div className=Css.sidebarPanel>
            <button
              className=Css.collapseButton
              title="Toggle default collapsed categories"
              onClick={event => {
                event->React.Event.Mouse.preventDefault;
                onToggleCollapsedCategoriesByDefault();
              }}>
              {if (isCategoriesCollapsedByDefault) {Icon.categoryCollapsed} else {
                 Icon.categoryExpanded
               }}
            </button>
            <SearchInput
              // SearchInput renders before any story, so there should be no race-condition
              // if a component in a story wants to take over focus
              autoFocus=true
              value={filterValue->Option.getWithDefault("")}
              onChange={event => {
                let value = event->React.Event.Form.target##value;

                setFilterValue(_ =>
                  if (value->Js.String.trim == "") {
                    None;
                  } else {
                    Some(value);
                  }
                );
              }}
              onClear={() => setFilterValue(_ => None)}
            />
          </div>
        </PaddedBox>
      </div>
      <PaddedBox gap=Xxs>
        {renderMenu(
           ~isCategoriesCollapsedByDefault,
           ~searchString=
             filterValue->Option.mapWithDefault("", Js.String.toLowerCase),
           ~urlSearchParams,
           demos,
         )}
      </PaddedBox>
    </Sidebar>;
  };
};

module DemoUnitSidebar = {
  module Css = {
    open StyleVars;

    let label = [%cx
      {|
      display: block;
      background-color: $(Color.white);
      border-radius: $(BorderRadius.default);
      box-shadow: 0 5px 10px 0 rgba(0, 0, 0, 0.07);
    |}
    ];

    let labelText = [%cx
      {|
      font-size: $(FontSize.md);
      text-align: center;
    |}
    ];

    let textInput = [%cx
      {|
      font-size: $(FontSize.md);
      width: 100%;
      box-sizing: border-box;
      background-color: $(Color.lightGray);
      box-shadow: inset 0 0 0 1px rgba(0, 0, 0, 0.1);
      border: none;
      padding: $(Gap.md);
      border-radius: $(BorderRadius.default);
    |}
    ];

    let select = [%cx
      {|
      font-size: $(FontSize.md);
      width: 100%;
      box-sizing: border-box;
      background-color: $(Color.lightGray);
      box-shadow: inset 0 0 0 1px rgba(0, 0, 0, 0.1);
      border: none;
      padding: $(Gap.md);
      border-radius: $(BorderRadius.default);
      appearance: none;
      -webkit-appearance: none;
      padding-right: 30px;
      background-image:
          url("data:image/svg+xml,%3Csvg width='36' height='36' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath stroke='%2342484E' stroke-width='2' d='M12.246 14.847l5.826 5.826 5.827-5.826' fill='none' fill-rule='evenodd' stroke-linecap='round' stroke-linejoin='round'/%3E%3C/svg%3E");
      background-position: center right;
      background-size: contain;
      background-repeat: no-repeat;
    |}
    ];

    let checkbox = [%cx
      {|
      font-size: $(FontSize.md);
      margin: 0 auto;
      display: block;
    |}
    ];
  };

  module PropBox = {
    [@react.component]
    let make = (~propName: string, ~children) =>
      <label className=Css.label>
        <PaddedBox>
          <Stack>
            <div className=Css.labelText> propName->React.string </div>
            children
          </Stack>
        </PaddedBox>
      </label>;
  };

  [@react.component]
  let make =
      (
        ~strings:
           Map.String.t(
             (
               Configs.stringConfig,
               string,
               option(array((string, string))),
             ),
           ),
        ~ints: Map.String.t((Configs.numberConfig(int), int)),
        ~floats: Map.String.t((Configs.numberConfig(float), float)),
        ~bools: Map.String.t((Configs.boolConfig, bool)),
        ~onStringChange,
        ~onIntChange,
        ~onFloatChange,
        ~onBoolChange,
      ) =>
    <PaddedBox gap=Md>
      <Stack>
        {strings
         ->Map.String.toArray
         ->Array.map(((propName, (_config, value, options))) =>
             <PropBox key=propName propName>
               {switch (options) {
                | None =>
                  <input
                    type_="text"
                    value
                    className=Css.textInput
                    onChange={event =>
                      onStringChange(
                        propName,
                        event->React.Event.Form.target##value,
                      )
                    }
                  />
                | Some(options) =>
                  <select
                    className=Css.select
                    onChange={event => {
                      let value = event->React.Event.Form.target##value;

                      onStringChange(propName, value);
                    }}>
                    {options
                     ->Array.map(((key, optionValue)) =>
                         <option
                           key
                           selected={value == optionValue}
                           value=optionValue>
                           key->React.string
                         </option>
                       )
                     ->React.array}
                  </select>
                }}
             </PropBox>
           )
         ->React.array}
        {ints
         ->Map.String.toArray
         ->Array.map(((propName, ({Configs.min, max, _}, value))) =>
             <PropBox key=propName propName>
               <input
                 type_="number"
                 min={string_of_int(min)}
                 max={string_of_int(max)}
                 value={string_of_int(value)}
                 className=Css.textInput
                 onChange={event =>
                   onIntChange(
                     propName,
                     event->React.Event.Form.target##value->int_of_string,
                   )
                 }
               />
             </PropBox>
           )
         ->React.array}
        {floats
         ->Map.String.toArray
         ->Array.map(((propName, ({Configs.min, max, _}, value))) =>
             <PropBox key=propName propName>
               <input
                 type_="number"
                 min={string_of_float(min)}
                 max={string_of_float(max)}
                 value={string_of_float(value)}
                 className=Css.textInput
                 onChange={event =>
                   onFloatChange(
                     propName,
                     event->React.Event.Form.target##value->float_of_string,
                   )
                 }
               />
             </PropBox>
           )
         ->React.array}
        {bools
         ->Map.String.toArray
         ->Array.map(((propName, (_config, checked))) =>
             <PropBox key=propName propName>
               <input
                 type_="checkbox"
                 checked
                 className=Css.checkbox
                 onChange={event =>
                   onBoolChange(
                     propName,
                     event->React.Event.Form.target##checked,
                   )
                 }
               />
             </PropBox>
           )
         ->React.array}
      </Stack>
    </PaddedBox>;
};

module DemoUnit = {
  module Css = {
    let container = [%cx
      {|
      flex-grow: 1;
      display: flex;
      align-items: stretch;
      flex-direction: row;
    |}
    ];

    let contents = [%cx
      {|
      flex-grow: 1;
      overflow-y: auto;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      -webkit-overflow-scrolling: touch;
    |}
    ];
  };

  type state = {
    strings:
      Map.String.t(
        (Configs.stringConfig, string, option(array((string, string)))),
      ),
    ints: Map.String.t((Configs.numberConfig(int), int)),
    floats: Map.String.t((Configs.numberConfig(float), float)),
    bools: Map.String.t((Configs.boolConfig, bool)),
  };

  type action =
    | SetString(string, string)
    | SetInt(string, int)
    | SetFloat(string, float)
    | SetBool(string, bool);

  let getRightSidebarElement = (): option(Dom.element) =>
    Window.window##parent##document##getElementById(rightSidebarId)
    ->Js.Nullable.toOption;

  [@react.component]
  let make = (~demoUnit: Configs.demoUnitProps => React.element) => {
    let (parentWindowRightSidebarElem, setParentWindowRightSidebarElem) =
      React.useState(() => None);

    React.useEffect0(() => {
      switch (getRightSidebarElement()) {
      | Some(elem) => setParentWindowRightSidebarElem(_ => Some(elem))
      | None => ()
      };
      None;
    });
    React.useEffect0(() => {
      Window.addMessageListener(event =>
        if (Window.window##parent === event##source) {
          let message: string = event##data;
          switch (message->Window.Message.fromStringOpt) {
          | Some(RightSidebarDisplayed) =>
            switch (getRightSidebarElement()) {
            | Some(elem) => setParentWindowRightSidebarElem(_ => Some(elem))
            | None => ()
            }
          | None => Js.Console.error("Unexpected message received")
          };
        }
      );
      None;
    });
    let (state, dispatch) =
      React.useReducer(
        (state, action) =>
          switch (action) {
          | SetString(name, newValue) => {
              ...state,
              strings:
                state.strings
                ->Map.String.update(name, value =>
                    value->Option.map(((config, _value, options)) =>
                      (config, newValue, options)
                    )
                  ),
            }
          | SetInt(name, newValue) => {
              ...state,
              ints:
                state.ints
                ->Map.String.update(name, value =>
                    value->Option.map(((config, _value)) =>
                      (config, newValue)
                    )
                  ),
            }
          | SetFloat(name, newValue) => {
              ...state,
              floats:
                state.floats
                ->Map.String.update(name, value =>
                    value->Option.map(((config, _value)) =>
                      (config, newValue)
                    )
                  ),
            }
          | SetBool(name, newValue) => {
              ...state,
              bools:
                state.bools
                ->Map.String.update(name, value =>
                    value->Option.map(((config, _value)) =>
                      (config, newValue)
                    )
                  ),
            }
          },
        {
          let strings = ref(Map.String.empty);
          let ints = ref(Map.String.empty);
          let floats = ref(Map.String.empty);
          let bools = ref(Map.String.empty);
          let props: Configs.demoUnitProps = {
            string: (name, ~options=?, config) => {
              strings :=
                strings.contents
                ->Map.String.set(name, (config, config, options));
              config;
            },
            int: (name, config) => {
              ints :=
                ints.contents->Map.String.set(name, (config, config.initial));
              config.initial;
            },
            float: (name, config) => {
              floats :=
                floats.contents
                ->Map.String.set(name, (config, config.initial));
              config.initial;
            },
            bool: (name, config) => {
              bools := bools.contents->Map.String.set(name, (config, config));
              config;
            },
          };

          let _ = demoUnit(props);
          {
            strings: strings.contents,
            ints: ints.contents,
            floats: floats.contents,
            bools: bools.contents,
          };
        },
      );

    let props: Configs.demoUnitProps = {
      string: (name, ~options as _=?, _config) => {
        let (_, value, _) = state.strings->Map.String.getExn(name);
        value;
      },
      int: (name, _config) => {
        let (_, value) = state.ints->Map.String.getExn(name);
        value;
      },
      float: (name, _config) => {
        let (_, value) = state.floats->Map.String.getExn(name);
        value;
      },
      bool: (name, _config) => {
        let (_, value) = state.bools->Map.String.getExn(name);
        value;
      },
    };

    <div name="DemoUnit" className=Css.container>
      <div className=Css.contents> {demoUnit(props)} </div>
      {switch (parentWindowRightSidebarElem) {
       | None => React.null
       | Some(element) =>
         ReactDOM.createPortal(
           <DemoUnitSidebar
             strings={state.strings}
             ints={state.ints}
             floats={state.floats}
             bools={state.bools}
             onStringChange={(name, value) =>
               dispatch(SetString(name, value))
             }
             onIntChange={(name, value) => dispatch(SetInt(name, value))}
             onFloatChange={(name, value) =>
               dispatch(SetFloat(name, value))
             }
             onBoolChange={(name, value) => dispatch(SetBool(name, value))}
           />,
           element,
         )
       }}
    </div>;
  };
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
      |}];
    };
  };

  let useFullframeUrl: bool = [%mel.raw
    {js|typeof USE_FULL_IFRAME_URL === "boolean" ? USE_FULL_IFRAME_URL : false|js}
  ];

  [@react.component]
  let make =
      (~queryString: string, ~responsiveMode, ~onLoad: Js.t('a) => unit) => {
    let iframePath = if (useFullframeUrl) {"demo/index.html"} else {"demo"};
    <div
      name="DemoUnitFrame"
      className={Css.container +++ Css.containerBackground(responsiveMode)}>
      <iframe
        className={Css.iframe(responsiveMode)}
        src={(iframePath ++ {js|?iframe=true&|js}) ++ queryString}
        onLoad={event => {
          let iframe = event->React.Event.Synthetic.target;
          let window = iframe##contentWindow;
          onLoad(window);
        }}
      />
    </div>;
  };
};

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
    | Unit(URLSearchParams.t, string)
    | Demo(string)
    | Home;

  [@react.component]
  let make = (~demos: Demos.t) => {
    let url = ReasonReactRouter.useUrl();
    let urlSearchParams = url.search->URLSearchParams.make;
    let route =
      switch (
        urlSearchParams->URLSearchParams.get("iframe"),
        urlSearchParams->URLSearchParams.get("demo"),
      ) {
      | (Some("true"), Some(demoName)) => Unit(urlSearchParams, demoName)
      | (_, Some(_)) => Demo(url.search)
      | _ => Home
      };

    let (loadedIframeWindow: option(Js.t('a)), setLoadedIframeWindow) =
      React.useState(() => None);

    let (iframeKey, setIframeKey) =
      React.useState(() => Js.Date.now()->Float.toString);

    React.useEffect1(
      () => {
        setIframeKey(_ => Js.Date.now()->Float.toString);
        None;
      },
      [|url|],
    );
    let (showRightSidebar, toggleShowRightSidebar) =
      React.useState(() =>
        LocalStorage.localStorage
        ->LocalStorage.getItem("sidebar")
        ->Option.isSome
      );

    let (responsiveMode, onSetResponsiveMode) = React.useState(() => Desktop);

    React.useEffect1(
      () => {
        if (showRightSidebar) {
          LocalStorage.localStorage->LocalStorage.setItem("sidebar", "1");
        } else {
          LocalStorage.localStorage->LocalStorage.removeItem("sidebar");
        };
        None;
      },
      [|showRightSidebar|],
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
       | Unit(urlSearchParams, demoName) =>
         let demoUnit = Demos.findDemo(urlSearchParams, demoName, demos);
         <div className=Css.main>
           {demoUnit
            ->Option.map(demoUnit => <DemoUnit demoUnit />)
            ->Option.getWithDefault("Demo not found"->React.string)}
         </div>;
       | Demo(queryString) =>
         <>
           <DemoListSidebar
             demos
             urlSearchParams
             isCategoriesCollapsedByDefault
             onToggleCollapsedCategoriesByDefault
           />
           <div name="Content" className=Css.right>
             <TopPanel
               isSidebarHidden={!showRightSidebar}
               responsiveMode
               onRightSidebarToggle={() => {
                 toggleShowRightSidebar(_ => !showRightSidebar);
                 switch (loadedIframeWindow) {
                 | Some(window) when !showRightSidebar =>
                   Window.postMessage(window, RightSidebarDisplayed)
                 | None
                 | _ => ()
                 };
               }}
               onSetResponsiveMode
             />
             <div name="Demo" className=Css.demo>
               <div className=Css.demoContents>
                 <DemoUnitFrame
                   key={"DemoUnitFrame" ++ iframeKey}
                   queryString
                   responsiveMode
                   onLoad={iframeWindow =>
                     setLoadedIframeWindow(_ => Some(iframeWindow))
                   }
                 />
               </div>
               {showRightSidebar
                  ? <Sidebar
                      key={"Sidebar" ++ iframeKey}
                      innerContainerId=rightSidebarId
                    />
                  : React.null}
             </div>
           </div>
         </>

       | Home =>
         <>
           <DemoListSidebar
             demos
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
