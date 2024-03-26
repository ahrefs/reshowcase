open Belt;
module Color = Layout.Color;
module Gap = Layout.Gap;
module Border = Layout.Border;
module BorderRadius = Layout.BorderRadius;
module FontSize = Layout.FontSize;
module PaddedBox = Layout.PaddedBox;
module Stack = Layout.Stack;
module Sidebar = Layout.Sidebar;
module Icon = Layout.Icon;
module Collapsible = Layout.Collapsible;
module URLSearchParams = Bindings.URLSearchParams;
module Window = Bindings.Window;
module LocalStorage = Bindings.LocalStorage;

type responsiveMode =
  | Mobile
  | Desktop;

module TopPanel = {
  module Styles = {
    let panel =
      ReactDOM.Style.make(
        ~display="flex",
        ~justifyContent="flex-end",
        ~borderBottom=Border.default,
        (),
      );

    let buttonGroup =
      ReactDOM.Style.make(
        ~overflow="hidden",
        ~display="flex",
        ~flexDirection="row",
        ~alignItems="stretch",
        ~borderRadius=BorderRadius.default,
        (),
      );

    let button =
      ReactDOM.Style.make(
        ~height="32px",
        ~width="48px",
        ~cursor="pointer",
        ~fontSize=FontSize.sm,
        ~backgroundColor=Color.lightGray,
        ~color=Color.darkGray,
        ~border="none",
        ~margin="0",
        ~padding="0",
        ~display="flex",
        ~alignItems="center",
        ~justifyContent="center",
        (),
      );

    let squareButton =
      button->(
                ReactDOM.Style.combine(ReactDOM.Style.make(~width="32px", ()))
              );

    let activeButton =
      button->(
                ReactDOM.Style.combine(
                  ReactDOM.Style.make(
                    ~backgroundColor=Color.blue,
                    ~color=Color.white,
                    (),
                  ),
                )
              );

    let middleSection =
      ReactDOM.Style.make(
        ~display="flex",
        ~flex="1",
        ~justifyContent="center",
        (),
      );

    let rightSection = ReactDOM.Style.make(~display="flex", ());
  };

  [@react.component]
  let make =
      (
        ~isSidebarHidden: bool,
        ~responsiveMode: responsiveMode,
        ~onRightSidebarToggle: unit => unit,
        ~onSetResponsiveMode: (responsiveMode => responsiveMode) => unit,
      ) =>
    <div style=Styles.panel>
      <div style=Styles.rightSection />
      <div style=Styles.middleSection>
        <PaddedBox gap=Md>
          <div style=Styles.buttonGroup>
            <button
              title="Show in desktop mode"
              style={
                      if (responsiveMode == Desktop) {
                        Styles.activeButton;
                      } else {
                        Styles.button;
                      }
                    }
              onClick={event => {
                event->React.Event.Mouse.preventDefault;
                onSetResponsiveMode(_ => Desktop);
              }}>
              Icon.desktop
            </button>
            <button
              title="Show in mobile mode"
              style={
                      if (responsiveMode == Mobile) {
                        Styles.activeButton;
                      } else {
                        Styles.button;
                      }
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
      <div style=Styles.rightSection>
        <PaddedBox gap=Md>
          <div style=Styles.buttonGroup>
            <button
              title={
                      if (isSidebarHidden) {"Show sidebar"} else {
                        "Hide sidebar"
                      }
                    }
              style=Styles.squareButton
              onClick={event => {
                event->React.Event.Mouse.preventDefault;
                onRightSidebarToggle();
              }}>
              <div
                style={ReactDOM.Style.make(
                  ~transition="200ms ease-in-out transform",
                  ~transform=
                    if (isSidebarHidden) {"rotate(0)"} else {"rotate(180deg)"},
                  (),
                )}>
                Icon.sidebar
              </div>
            </button>
          </div>
        </PaddedBox>
      </div>
    </div>;
};

let rightSidebarId = "rightSidebar";

module Link = {
  [@react.component]
  let make = (~href, ~text: React.element, ~style=?, ~activeStyle=?) => {
    let url = ReasonReactRouter.useUrl();
    let path = String.concat("/", url.path);
    let isActive =
      Js.String.endsWith(~suffix=href, path ++ "?" ++ url.search);
    <a
      href
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
      }
      style=?{
        switch (style, activeStyle, isActive) {
        | (Some(style), _, false) => Some(style)
        | (Some(style), None, true) => Some(style)
        | (Some(style), Some(activeStyle), true) =>
          Some(ReactDOM.Style.combine(style, activeStyle))
        | (_, Some(activeStyle), true) => Some(activeStyle)
        | _ => None
        }
      }>
      text
    </a>;
  };
};

module DemoListSidebar = {
  module Styles = {
    let categoryName =
      ReactDOM.Style.make(
        ~padding=(Gap.xs ++ {js| |js}) ++ Gap.xxs,
        ~fontSize=FontSize.md,
        ~fontWeight="500",
        (),
      );

    let link =
      ReactDOM.Style.make(
        ~textDecoration="none",
        ~color=Color.blue,
        ~display="block",
        ~padding=(Gap.xs ++ {js| |js}) ++ Gap.md,
        ~borderRadius=BorderRadius.default,
        ~fontSize=FontSize.md,
        ~fontWeight="500",
        (),
      );

    let activeLink = ReactDOM.Style.make(~backgroundColor=Color.midGray, ());
  };

  module SearchInput = {
    module Styles = {
      let clearButton =
        ReactDOM.Style.make(
          ~position="absolute",
          ~right="7px",
          ~display="flex",
          ~cursor="pointer",
          ~border="none",
          ~padding="0",
          ~backgroundColor=Color.transparent,
          ~top="50%",
          ~transform="translateY(-50%)",
          ~margin="0",
          (),
        );

      let inputWrapper =
        ReactDOM.Style.make(
          ~position="relative",
          ~display="flex",
          ~alignItems="center",
          ~backgroundColor=Color.midGray,
          ~borderRadius=BorderRadius.default,
          (),
        );

      let input =
        ReactDOM.Style.make(
          ~padding=(Gap.xs ++ {js| |js}) ++ Gap.md,
          ~width="100%",
          ~margin="0",
          ~height="32px",
          ~boxSizing="border-box",
          ~fontFamily="inherit",
          ~fontSize=FontSize.md,
          ~border="none",
          ~backgroundColor=Color.transparent,
          ~borderRadius=BorderRadius.default,
          (),
        );
    };

    module ClearButton = {
      [@react.component]
      let make = (~onClear) =>
        <button style=Styles.clearButton onClick={_event => onClear()}>
          Icon.close
        </button>;
    };

    [@react.component]
    let make = (~value, ~onChange, ~onClear) =>
      <div style=Styles.inputWrapper>
        <input style=Styles.input placeholder="Filter" value onChange />
        {if (value == "") {
           React.null;
         } else {
           <ClearButton onClear />;
         }}
      </div>;
  };

  let renderMenu =
      (
        ~isCategoriesCollapsedByDefault: bool,
        ~urlSearchParams: URLSearchParams.t,
        ~searchString,
        demos: Demos.t,
      ) => {
    let rec renderMenu =
            (
              ~parentCategoryMatchedSearch: bool,
              ~nestingLevel,
              ~categoryQuery,
              demos: Demos.t,
            ) => {
      let demos = demos->Js.Dict.entries;
      demos
      ->(
          Array.map(((entityName, entity)) => {
            let searchMatchingTerms =
              HighlightTerms.getMatchingTerms(~searchString, ~entityName);

            let isEntityNameMatchSearch =
              searchString == "" || searchMatchingTerms->Belt.Array.size > 0;

            switch (entity) {
            | Entity.Demo(_) =>
              if (isEntityNameMatchSearch || parentCategoryMatchedSearch) {
                <Link
                  key=entityName
                  style=Styles.link
                  activeStyle=Styles.activeLink
                  href={
                    ("?demo=" ++ entityName->Js.Global.encodeURIComponent)
                    ++ categoryQuery
                  }
                  text={
                    <HighlightTerms
                      text=entityName
                      terms=searchMatchingTerms
                    />
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
                    urlSearchParams->(URLSearchParams.get(categoryQueryKey))
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
                      <div style=Styles.categoryName>
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
                           isEntityNameMatchSearch
                           || parentCategoryMatchedSearch,
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
        )
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
      <PaddedBox gap=Md border=Bottom>
        <div
          style={ReactDOM.Style.make(
            ~display="flex",
            ~alignItems="center",
            ~gridGap="5px",
            (),
          )}>
          <button
            style={ReactDOM.Style.make(
              ~height="32px",
              ~minWidth="32px",
              ~width="32px",
              ~cursor="pointer",
              ~fontSize=FontSize.sm,
              ~backgroundColor=Color.white,
              ~color=Color.darkGray,
              ~border=Border.default,
              ~borderRadius=BorderRadius.default,
              ~margin="0",
              ~padding="0",
              ~display="flex",
              ~alignItems="center",
              ~justifyContent="center",
              (),
            )}
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
            value={filterValue->(Option.getWithDefault(""))}
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
      <PaddedBox gap=Xxs>
        {renderMenu(
           ~isCategoriesCollapsedByDefault,
           ~searchString=
             filterValue->(Option.mapWithDefault("", Js.String.toLowerCase)),
           ~urlSearchParams,
           demos,
         )}
      </PaddedBox>
    </Sidebar>;
  };
};

module DemoUnitSidebar = {
  module Styles = {
    let label =
      ReactDOM.Style.make(
        ~display="block",
        ~backgroundColor=Color.white,
        ~borderRadius=BorderRadius.default,
        ~boxShadow="0 5px 10px rgba(0, 0, 0, 0.07)",
        (),
      );

    let labelText =
      ReactDOM.Style.make(~fontSize=FontSize.md, ~textAlign="center", ());

    let textInput =
      ReactDOM.Style.make(
        ~fontSize=FontSize.md,
        ~width="100%",
        ~boxSizing="border-box",
        ~backgroundColor=Color.lightGray,
        ~boxShadow="inset 0 0 0 1px rgba(0, 0, 0, 0.1)",
        ~border="none",
        ~padding=Gap.md,
        ~borderRadius=BorderRadius.default,
        (),
      );

    let select =
      ReactDOM.Style.make(
        ~fontSize=FontSize.md,
        ~width="100%",
        ~boxSizing="border-box",
        ~backgroundColor=Color.lightGray,
        ~boxShadow="inset 0 0 0 1px rgba(0, 0, 0, 0.1)",
        ~border="none",
        ~padding=Gap.md,
        ~borderRadius=BorderRadius.default,
        ~appearance="none",
        ~paddingRight="30px",
        ~backgroundImage=
          {js|url("data:image/svg+xml,%3Csvg width='36' height='36' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath stroke='%2342484E' stroke-width='2' d='M12.246 14.847l5.826 5.826 5.827-5.826' fill='none' fill-rule='evenodd' stroke-linecap='round' stroke-linejoin='round'/%3E%3C/svg%3E")|js},
        ~backgroundPosition="center right",
        ~backgroundSize="contain",
        ~backgroundRepeat="no-repeat",
        (),
      )
      ->(ReactDOM.Style.unsafeAddProp("WebkitAppearance", "none"));

    let checkbox =
      ReactDOM.Style.make(
        ~fontSize=FontSize.md,
        ~margin="0 auto",
        ~display="block",
        (),
      );
  };

  module PropBox = {
    [@react.component]
    let make = (~propName: string, ~children) =>
      <label style=Styles.label>
        <PaddedBox>
          <Stack>
            <div style=Styles.labelText> propName->React.string </div>
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
         ->(
             Array.map(((propName, (_config, value, options))) =>
               <PropBox key=propName propName>
                 {switch (options) {
                  | None =>
                    <input
                      type_="text"
                      value
                      style=Styles.textInput
                      onChange={event =>
                        onStringChange(
                          propName,
                          event->React.Event.Form.target##value,
                        )
                      }
                    />
                  | Some(options) =>
                    <select
                      style=Styles.select
                      onChange={event => {
                        let value = event->React.Event.Form.target##value;

                        onStringChange(propName, value);
                      }}>
                      {options
                       ->(
                           Array.map(((key, optionValue)) =>
                             <option
                               key
                               selected={value == optionValue}
                               value=optionValue>
                               key->React.string
                             </option>
                           )
                         )
                       ->React.array}
                    </select>
                  }}
               </PropBox>
             )
           )
         ->React.array}
        {ints
         ->Map.String.toArray
         ->(
             Array.map(((propName, ({Configs.min, max, _}, value))) =>
               <PropBox key=propName propName>
                 <input
                   type_="number"
                   min={string_of_int(min)}
                   max={string_of_int(max)}
                   value={string_of_int(value)}
                   style=Styles.textInput
                   onChange={event =>
                     onIntChange(
                       propName,
                       event->React.Event.Form.target##value->int_of_string,
                     )
                   }
                 />
               </PropBox>
             )
           )
         ->React.array}
        {floats
         ->Map.String.toArray
         ->(
             Array.map(((propName, ({Configs.min, max, _}, value))) =>
               <PropBox key=propName propName>
                 <input
                   type_="number"
                   min={string_of_float(min)}
                   max={string_of_float(max)}
                   value={string_of_float(value)}
                   style=Styles.textInput
                   onChange={event =>
                     onFloatChange(
                       propName,
                       event->React.Event.Form.target##value->float_of_string,
                     )
                   }
                 />
               </PropBox>
             )
           )
         ->React.array}
        {bools
         ->Map.String.toArray
         ->(
             Array.map(((propName, (_config, checked))) =>
               <PropBox key=propName propName>
                 <input
                   type_="checkbox"
                   checked
                   style=Styles.checkbox
                   onChange={event =>
                     onBoolChange(
                       propName,
                       event->React.Event.Form.target##checked,
                     )
                   }
                 />
               </PropBox>
             )
           )
         ->React.array}
      </Stack>
    </PaddedBox>;
};

module DemoUnit = {
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

  module Styles = {
    let container =
      ReactDOM.Style.make(
        ~flexGrow="1",
        ~display="flex",
        ~alignItems="stretch",
        ~flexDirection="row",
        (),
      );

    let contents =
      ReactDOM.Style.make(
        ~flexGrow="1",
        ~overflowY="auto",
        ~display="flex",
        ~flexDirection="column",
        ~alignItems="center",
        ~justifyContent="center",
        (),
      )
      ->(ReactDOM.Style.unsafeAddProp("WebkitOverflowScrolling", "touch"));
  };

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
                ->(
                    Map.String.update(name, value =>
                      value->(
                               Option.map(((config, _value, options)) =>
                                 (config, newValue, options)
                               )
                             )
                    )
                  ),
            }
          | SetInt(name, newValue) => {
              ...state,
              ints:
                state.ints
                ->(
                    Map.String.update(name, value =>
                      value->(
                               Option.map(((config, _value)) =>
                                 (config, newValue)
                               )
                             )
                    )
                  ),
            }
          | SetFloat(name, newValue) => {
              ...state,
              floats:
                state.floats
                ->(
                    Map.String.update(name, value =>
                      value->(
                               Option.map(((config, _value)) =>
                                 (config, newValue)
                               )
                             )
                    )
                  ),
            }
          | SetBool(name, newValue) => {
              ...state,
              bools:
                state.bools
                ->(
                    Map.String.update(name, value =>
                      value->(
                               Option.map(((config, _value)) =>
                                 (config, newValue)
                               )
                             )
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
                ->(Map.String.set(name, (config, config, options)));
              config;
            },
            int: (name, config) => {
              ints :=
                ints.contents
                ->(Map.String.set(name, (config, config.initial)));
              config.initial;
            },
            float: (name, config) => {
              floats :=
                floats.contents
                ->(Map.String.set(name, (config, config.initial)));
              config.initial;
            },
            bool: (name, config) => {
              bools :=
                bools.contents->(Map.String.set(name, (config, config)));
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
        let (_, value, _) = state.strings->(Map.String.getExn(name));
        value;
      },
      int: (name, _config) => {
        let (_, value) = state.ints->(Map.String.getExn(name));
        value;
      },
      float: (name, _config) => {
        let (_, value) = state.floats->(Map.String.getExn(name));
        value;
      },
      bool: (name, _config) => {
        let (_, value) = state.bools->(Map.String.getExn(name));
        value;
      },
    };

    <div name="DemoUnit" style=Styles.container>
      <div style=Styles.contents> {demoUnit(props)} </div>
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
  let container = responsiveMode =>
    ReactDOM.Style.make(
      ~flex="1",
      ~display="flex",
      ~justifyContent="center",
      ~alignItems="center",
      ~backgroundColor=
        switch (responsiveMode) {
        | Mobile => Color.midGray
        | Desktop => Color.white
        },
      ~height="1px",
      ~overflowY="auto",
      (),
    );

  let useFullframeUrl: bool = [%mel.raw
    {js|typeof USE_FULL_IFRAME_URL === "boolean" ? USE_FULL_IFRAME_URL : false|js}
  ];

  [@react.component]
  let make =
      (
        ~queryString as _: string,
        ~responsiveMode,
        ~onLoad: Js.t('a) => unit,
        ~children,
      ) => {
    let (body, setBody) = React.useState(_ => None);
    let _iframePath = if (useFullframeUrl) {"demo/index.html"} else {"demo"};

    <div name="DemoUnitFrame" style={container(responsiveMode)}>
      <iframe
        style={ReactDOM.Style.make(
          ~height=
            switch (responsiveMode) {
            | Mobile => "667px"
            | Desktop => "100%"
            },
          ~width=
            switch (responsiveMode) {
            | Mobile => "375px"
            | Desktop => "100%"
            },
          ~border="none",
          (),
        )}
        onLoad={event => {
          let iframe = event->React.Event.Synthetic.target;
          let body =
            iframe##contentWindow##document->Option.flatMap(d => d##body);
          setBody(_ => body);
          onLoad(iframe##contentWindow);
        }}>
        {switch (body) {
         | None => React.null
         | Some(body) => ReactDOM.createPortal(children, body)
         }}
      </iframe>
    </div>;
  };
};

module App = {
  module Styles = {
    let app =
      ReactDOM.Style.make(
        ~display="flex",
        ~flexDirection="row",
        ~minHeight="100vh",
        ~alignItems="stretch",
        ~color=Color.darkGray,
        (),
      );

    let main =
      ReactDOM.Style.make(
        ~flexGrow="1",
        ~display="flex",
        ~flexDirection="column",
        (),
      );

    let empty =
      ReactDOM.Style.make(
        ~flexGrow="1",
        ~display="flex",
        ~flexDirection="column",
        ~alignItems="center",
        ~justifyContent="center",
        (),
      );

    let emptyText =
      ReactDOM.Style.make(
        ~fontSize=FontSize.lg,
        ~color=Color.black40a,
        ~textAlign="center",
        (),
      );

    let right =
      ReactDOM.Style.make(
        ~display="flex",
        ~flexDirection="column",
        ~width="100%",
        (),
      );

    let demo =
      ReactDOM.Style.make(
        ~display="flex",
        ~flex="1",
        ~flexDirection="row",
        ~alignItems="stretch",
        (),
      );

    let demoContents =
      ReactDOM.Style.make(
        ~display="flex",
        ~flex="1",
        ~flexDirection="column",
        (),
      );
  };

  type route =
    | Unit(URLSearchParams.t, string)
    | Demo(string, string)
    | Home;

  [@react.component]
  let make = (~demos: Demos.t) => {
    let url = ReasonReactRouter.useUrl();
    let urlSearchParams = url.search->URLSearchParams.make;
    let route =
      switch (
        urlSearchParams->(URLSearchParams.get("iframe")),
        urlSearchParams->(URLSearchParams.get("demo")),
      ) {
      | (Some("true"), Some(demoName)) => Unit(urlSearchParams, demoName)
      | (_, Some(demoName)) => Demo(url.search, demoName)
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
        ->(LocalStorage.getItem("sidebar"))
        ->Option.isSome
      );

    let (responsiveMode, onSetResponsiveMode) = React.useState(() => Desktop);

    React.useEffect1(
      () => {
        if (showRightSidebar) {
          LocalStorage.localStorage->(LocalStorage.setItem("sidebar", "1"));
        } else {
          LocalStorage.localStorage->(LocalStorage.removeItem("sidebar"));
        };
        None;
      },
      [|showRightSidebar|],
    );
    let (isCategoriesCollapsedByDefault, toggleIsCategoriesCollapsed) =
      React.useState(() =>
        switch (
          LocalStorage.localStorage->(
                                       LocalStorage.getItem(
                                         "isCategoriesCollapsedByDefault",
                                       )
                                     )
        ) {
        | Some("true") => true
        | _ => false
        }
      );

    let onToggleCollapsedCategoriesByDefault = () => {
      toggleIsCategoriesCollapsed(_ => !isCategoriesCollapsedByDefault);
      LocalStorage.localStorage->(
                                   LocalStorage.setItem(
                                     "isCategoriesCollapsedByDefault",
                                     if (!isCategoriesCollapsedByDefault) {
                                       "true";
                                     } else {
                                       "false";
                                     },
                                   )
                                 );
    };

    <div name="App" style=Styles.app>
      {switch (route) {
       | Unit(_, _) => React.null
       | Demo(_, _)
       | Home =>
         <DemoListSidebar
           demos
           urlSearchParams
           isCategoriesCollapsedByDefault
           onToggleCollapsedCategoriesByDefault
         />
       }}
      {switch (route) {
       | Unit(urlSearchParams, demoName) =>
         let demoUnit = Demos.findDemo(urlSearchParams, demoName, demos);
         <div style=Styles.main>
           {demoUnit
            ->(Option.map(demoUnit => <DemoUnit demoUnit />))
            ->(Option.getWithDefault("Demo not found"->React.string))}
         </div>;
       | Demo(queryString, demoName) =>
         let demoUnit =
           Demos.findDemo(urlSearchParams, demoName, demos)
           ->(Option.map(demoUnit => <DemoUnit demoUnit />))
           ->(Option.getWithDefault("Demo not found"->React.string));
         <div name="Content" style=Styles.right>
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
           <div name="Demo" style=Styles.demo>
             <div style=Styles.demoContents>
               <DemoUnitFrame
                 queryString
                 responsiveMode
                 onLoad={iframeWindow =>
                   setLoadedIframeWindow(_ => Some(iframeWindow))
                 }>
                 demoUnit
               </DemoUnitFrame>
             </div>
             {if (showRightSidebar) {
                <Sidebar
                  key={"Sidebar" ++ iframeKey}
                  innerContainerId=rightSidebarId
                />;
              } else {
                React.null;
              }}
           </div>
         </div>;
       | Home =>
         <div style=Styles.empty>
           <div style=Styles.emptyText> "Pick a demo"->React.string </div>
         </div>
       }}
    </div>;
  };
};
