let modulePath = Reshowcase.Utils.getFilepath();
let demoName = "Paragraph";

[@react.component]
let make = () => {
  <p> {React.string("hello")} </p>;
};
