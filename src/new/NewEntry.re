let makeDemoTemplate = (~filepath: string) => {j|
import * as Demo from "$(filepath)";
import * as Client from "react-dom/client";
import * as JsxRuntime from "react/jsx-runtime";

const root = document.querySelector("#root");

if (!(root == null)) {
  const root1 = Client.createRoot(root);
  root1.render(JsxRuntime.jsx(Demo.make, {}));
}
|j};

type extractedDemo = {
  filepath: string,
  targetPath: list(string),
};

let targetPathToFilepath = targetPath => {
  let path =
    targetPath
    ->List.rev
    ->Belt.List.map(Js.String.toLowerCase)
    ->Belt.List.toArray
    ->Js.Array.join(~sep="/", _);

  path ++ ".js";
};

let extractDemos = (~items: array(NewEntity.item)): list(extractedDemo) => {
  let rec extractWithPath =
          (~path: list(string), ~items: array(NewEntity.item))
          : list(extractedDemo) => {
    Js.Array.reduce(
      ~f=
        (acc, item) => {
          switch (item) {
          | NewEntity.Demo(demo) =>
            let targetPath = [demo.name, ...path];
            let extracted = {
              filepath: demo.modulePath,
              targetPath,
            };
            [extracted, ...acc];
          | NewEntity.Category(category) =>
            let nestedDemos =
              extractWithPath(
                ~path=[category.name, ...path],
                ~items=category.items,
              );
            List.append(nestedDemos, acc);
          }
        },
      ~init=[],
      items,
    );
  };

  extractWithPath(~path=[], ~items);
};

let start = (~items: array(NewEntity.item)) => {
  let demos = extractDemos(~items);
  let finalFilepaths =
    demos->Belt.List.map(extractedDemo =>
      targetPathToFilepath(extractedDemo.targetPath)
    );
  Js.log2("!!! demos:\n", Util.inspect(demos));
  Js.log2(
    "!!! finalFilepaths:\n",
    Util.inspect(finalFilepaths->Belt.List.toArray),
  );
};
