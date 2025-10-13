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

let replaceByRe = (s, regexp, replacement) =>
  Js.String.replaceByRe(~regexp, ~replacement, s);

let slugify = text => {
  text
  ->Js.String.toLowerCase
  ->Js.String.trim
  ->replaceByRe([%re "/\\s+/g"], "-") // Replace spaces with `-`
  ->replaceByRe([%re "/[^\\w-]+/g"], "") // Remove all non-word chars
  ->replaceByRe([%re "/--+/g"], "-"); // Replace multiple `-` with single `-`
};

let targetPathToFilepath = targetPath => {
  let path =
    targetPath
    ->List.rev
    ->Belt.List.map(slugify)
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

  let () = {
    demos->Belt.List.forEach(extractedDemo => {
      let prefix = "/Users/denstr/projects/reshowcase/build";
      let finalFilepath = Path.join2(prefix, targetPathToFilepath(extractedDemo.targetPath));
      let template = makeDemoTemplate(~filepath=extractedDemo.filepath);
      let () = Fs.mkDirSync(Path.dirname(finalFilepath), {recursive: true});
      Fs.writeFileSync(
        ~path=finalFilepath,
        ~data=template,
      );
    });
  };

  // Js.log2("!!! demos:\n", Util.inspect(demos));
  // Js.log2(
  //   "!!! finalFilepaths:\n",
  //   Util.inspect(finalFilepaths->Belt.List.toArray),
  // );
};
