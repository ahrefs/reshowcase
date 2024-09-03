open Prelude;

module PaddedBox = {
  type padding =
    | Around
    | LeftRight
    | TopLeftRight;

  type border =
    | None
    | Bottom;

  type gap =
    | Xxs
    | Xs
    | Md;

  module Css = {
    open StyleVars;

    let boxPadding = (~paddingType, ~gap) => {
      let paddingSize =
        switch (gap) {
        | Xxs => Gap.xxs
        | Xs => Gap.xs
        | Md => Gap.md
        };
      switch (paddingType) {
      | Around => [%cx {| padding: $(paddingSize) |}]
      | LeftRight => [%cx {| padding: 0 $(paddingSize) |}]
      | TopLeftRight => [%cx {| padding: $(paddingSize) $(paddingSize) 0 |}]
      };
    };

    let boxBorder =
      fun
      | None => [%cx ""]
      | Bottom => [%cx {| border-bottom: 1px solid $(Color.midGray); |}];
  };

  [@react.component]
  let make =
      (
        ~gap: gap=Xs,
        ~padding: padding=Around,
        ~border: border=None,
        ~id=?,
        ~children,
      ) =>
    <div
      name="PaddedBox"
      ?id
      className={
        Css.boxPadding(~paddingType=padding, ~gap) +++ Css.boxBorder(border)
      }>
      children
    </div>;
};

module Stack = {
  module Css = {
    open StyleVars;

    let stack = [%cx
      {|
        display: grid;
        grid-gap: $(Gap.xs);
      |}
    ];
  };

  [@react.component]
  let make = (~children) =>
    <div name="Stack" className=Css.stack> children </div>;
};

module Sidebar = {
  module Css = {
    open StyleVars;

    let width = `px(230);

    let sidebar = [%cx
      {|
      min-width: $(width);
      width: $(width);
      overflow-y: auto;
      background-color: $(Color.lightGray);
      -webkit-overflow-scrolling: touch;
    |}
    ];

    let sidebarFullHeight = [%cx {|
      height: 100vh;
    |}];
  };

  [@react.component]
  let make = (~innerContainerId=?, ~fullHeight=false, ~children=React.null) =>
    <div
      name="Sidebar"
      id=?innerContainerId
      className={Css.sidebar +++ Css.sidebarFullHeight->Cn.ifTrue(fullHeight)}>
      children
    </div>;
};

module Icon = {
  open StyleVars;

  module Css = {
    let iconBlock = [%cx {|
      display: block;
    |}];
  };

  let desktop =
    <svg width="32" height="32">
      <g transform="translate(5 8)" fill="none" fillRule="evenodd">
        <rect stroke="currentColor" x="2" width="18" height="13" rx="1" />
        <rect fill="currentColor" y="13" width="22" height="2" rx="1" />
      </g>
    </svg>;

  let mobile =
    <svg width="32" height="32">
      <g transform="translate(11 7)" fill="none" fillRule="evenodd">
        <rect stroke="currentColor" width="10" height="18" rx="2" />
        <path
          d="M2 0h6v1a1 1 0 01-1 1H3a1 1 0 01-1-1V0z"
          fill="currentColor"
        />
      </g>
    </svg>;

  let sidebar =
    <svg width="32" height="32">
      <g
        stroke="currentColor"
        strokeWidth="1.5"
        fill="none"
        fillRule="evenodd"
        strokeLinecap="round"
        strokeLinejoin="round">
        <path
          d="M25.438 17H12.526M19 10.287L12.287 17 19 23.713M8.699 7.513v17.2"
        />
      </g>
    </svg>;

  let close =
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width="18"
      height="18"
      viewBox="0 0 18 18"
      className=Css.iconBlock>
      <path
        fill="gray"
        d="M14.53 4.53l-1.06-1.06L9 7.94 4.53 3.47 3.47 4.53 7.94 9l-4.47 4.47 1.06 1.06L9 10.06l4.47 4.47 1.06-1.06L10.06 9z"
      />
    </svg>;

  let categoryCollapsed =
    <svg
      width="20"
      height="17"
      viewBox="0 0 20 17"
      fill={Color.darkGray->Color.toString}
      xmlns="http://www.w3.org/2000/svg">
      <rect x="2" y="1" width="16" height="2" />
      <rect x="2" y="7" width="16" height="2" />
      <rect x="2" y="13" width="16" height="2" />
    </svg>;

  let categoryExpanded =
    <svg
      width="26"
      height="17"
      viewBox="0 0 26 17"
      fill={Color.darkGray->Color.toString}
      xmlns="http://www.w3.org/2000/svg">
      <rect x="6" y="1" width="16" height="2" />
      <rect x="2" y="1" width="2" height="2" />
      <rect x="10" y="7" width="12" height="2" />
      <rect x="6" y="7" width="2" height="2" />
      <rect x="10" y="13" width="12" height="2" />
      <rect x="6" y="13" width="2" height="2" />
    </svg>;
};

module Collapsible = {
  module Css = {
    let clickableArea = [%cx
      {|
      display: flex;
      cursor: pointer;
      grid-gap: 2px;
      align-items: center;
    |}
    ];

    let icon = [%cx
      {|
      transition: 200ms ease-out transform;
      transform: rotate(-90deg);
    |}
    ];

    let iconActive = [%cx {|
      transform: rotate(0);
    |}];
  };

  let triangleIcon = isOpen =>
    <svg
      width="10"
      height="6"
      className={Css.icon +++ Css.iconActive->Cn.ifTrue(isOpen)}>
      <polygon
        points="0,0  10,0  5,6"
        fill=StyleVars.Color.(darkGray->toString)
      />
    </svg>;

  [@react.component]
  let make =
      (
        ~title: React.element,
        ~isDefaultOpen: bool=false,
        ~isForceOpen=false,
        ~children,
      ) => {
    let (isOpen, setIsOpen) = React.useState(() => isDefaultOpen);
    React.useEffect1(
      () => {
        setIsOpen(_ => isDefaultOpen);
        None;
      },
      [|isDefaultOpen|],
    );
    <div>
      <div
        className=Css.clickableArea
        onClick={_event => setIsOpen(isOpen => !isOpen)}>
        {triangleIcon(isOpen)}
        title
      </div>
      {isForceOpen || isOpen ? children : React.null}
    </div>;
  };
};
