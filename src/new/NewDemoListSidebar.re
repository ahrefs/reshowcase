open Belt;
open Prelude;
open Layout;
module URLSearchParams = Bindings.URLSearchParams;
module Window = Bindings.Window;
module LocalStorage = Bindings.LocalStorage;

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
    let currentPath = "/" ++ String.concat("/", url.path);
    let isActive = currentPath == href;

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

let rec isNestedEntityMatchSearch =
        (demos: array(NewEntity.item), searchString) =>
  demos->Array.some((item: NewEntity.item) => {
    switch (item) {
    | Demo({name: demoName, modulePath: _}) =>
      let isEntityNameMatchSearch =
        HighlightTerms.getMatchingTerms(~searchString, ~entityName=demoName)
        ->Array.size
        > 0;

      isEntityNameMatchSearch;
    | Category({name: categoryName, items}) =>
      let isEntityNameMatchSearch =
        HighlightTerms.getMatchingTerms(
          ~searchString,
          ~entityName=categoryName,
        )
        ->Array.size
        > 0;

      isEntityNameMatchSearch
      || isNestedEntityMatchSearch(items, searchString);
    }
  });

let renderMenu =
    (
      ~isCategoriesCollapsedByDefault: bool,
      ~searchString,
      ~url: ReasonReactRouter.url,
      ~publicPath: string,
      items: array(NewEntity.item),
    ) => {
  let activeElementRef = UseScrollIntoView.use();

  let rec renderMenu =
          (
            ~parentCategoryMatchedSearch: bool,
            ~nestingLevel,
            ~categoryPath: list(string),
            items: array(NewEntity.item),
          ) => {
    items
    ->Array.map((item: NewEntity.item) => {
        switch (item) {
        | NewEntity.Demo({name: demoName, modulePath: _}) =>
          let searchMatchingTerms =
            HighlightTerms.getMatchingTerms(
              ~searchString,
              ~entityName=demoName,
            );

          let isEntityNameMatchSearch =
            searchString == "" || searchMatchingTerms->Belt.Array.size > 0;

          if (isEntityNameMatchSearch || parentCategoryMatchedSearch) {
            let publicPathSegments =
              publicPath
              ->Js.String.split(~sep="/", _)
              ->Belt.Array.keep(segment => segment != "");

            let demoPathSegments =
              Belt.List.concat(categoryPath, [demoName])
              ->Belt.List.map(Utils.slugify)
              ->Belt.List.toArray;

            let fullPathSegments =
              Belt.Array.concat(publicPathSegments, demoPathSegments);

            let href = "/" ++ Js.Array.join(~sep="/", fullPathSegments);

            <SidebarLink
              activeDomRef=activeElementRef
              key=demoName
              href
              text={<HighlightTerms text=demoName terms=searchMatchingTerms />}
            />;
          } else {
            React.null;
          };
        | Category({name: categoryName, items}) =>
          let searchMatchingTerms =
            HighlightTerms.getMatchingTerms(
              ~searchString,
              ~entityName=categoryName,
            );

          let isEntityNameMatchSearch =
            searchString == "" || searchMatchingTerms->Belt.Array.size > 0;

          if ((
                isEntityNameMatchSearch
                || isNestedEntityMatchSearch(items, searchString)
              )
              || parentCategoryMatchedSearch) {
            let currentPath = Belt.List.concat(categoryPath, [categoryName]);
            let currentPathString =
              currentPath
              ->Belt.List.map(Utils.slugify)
              ->Belt.List.toArray
              ->Js.Array.join(~sep="/", _);

            let isCategoryInCurrentPath = {
              let urlPath = "/" ++ String.concat("/", url.path);
              Js.String.startsWith(~prefix="/" ++ currentPathString, urlPath);
            };

            <PaddedBox key=categoryName padding=LeftRight>
              <Collapsible
                title={
                  <div className=Css.categoryName>
                    <HighlightTerms
                      text=categoryName
                      terms=searchMatchingTerms
                    />
                  </div>
                }
                isDefaultOpen={
                  isCategoryInCurrentPath || !isCategoriesCollapsedByDefault
                }
                isForceOpen={searchString != ""}>
                <PaddedBox padding=LeftRight>
                  {renderMenu(
                     ~parentCategoryMatchedSearch=
                       isEntityNameMatchSearch || parentCategoryMatchedSearch,
                     ~nestingLevel=nestingLevel + 1,
                     ~categoryPath=currentPath,
                     items,
                   )}
                </PaddedBox>
              </Collapsible>
            </PaddedBox>;
          } else {
            React.null;
          };
        }
      })
    ->React.array;
  };

  renderMenu(
    ~parentCategoryMatchedSearch=false,
    ~nestingLevel=0,
    ~categoryPath=[],
    items,
  );
};

[@react.component]
let make =
    (
      ~items: array(NewEntity.item),
      ~url: ReasonReactRouter.url,
      ~isCategoriesCollapsedByDefault: bool,
      ~onToggleCollapsedCategoriesByDefault: unit => unit,
      ~publicPath: string,
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
         ~url,
         ~publicPath,
         items,
       )}
    </PaddedBox>
  </Sidebar>;
};
