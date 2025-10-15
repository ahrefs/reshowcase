let modulePath = Reshowcase.Utils.getFilepath();
let demoName = "Italic";

[@react.component]
let make = () => {
  <i> {React.string("hello")} </i>;
};
