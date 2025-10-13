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

let makeMainTemplate = (~filepath: string, ~items: array(NewEntity.item)) => {
  let itemsJsonString = items->NewEntity.items_to_json_string;
  {j|
import * as Demo from "$(filepath)";
import * as Client from "react-dom/client";
import * as JsxRuntime from "react/jsx-runtime";

const root = document.querySelector("#root");

const itemsJsonString = $(itemsJsonString);

if (!(root == null)) {
  const root1 = Client.createRoot(root);
  root1.render(JsxRuntime.jsx(Demo.make, {itemsJsonString: itemsJsonString}));
}
|j};
};

let htmlTemplate = {js|
<!DOCTYPE html>
<html>
  <body>
    <div id="root"></div>
  </body>
</html>
|js};

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

let entriesOutputDir = "/Users/denstr/projects/reshowcase/build";

let esbuildOutputDir = Path.join2(entriesOutputDir, "esbuild");

let start = (~items: array(NewEntity.item)) => {
  let _demos = extractDemos(~items);

  let mainEntryModulePath = NewReshowcaseUi2.modulePath;

  let mainEntryJsPath = Path.join2(entriesOutputDir, "main.js");
  let mainEntryHtmlPath = Path.join2(entriesOutputDir, "index.html");

  let () = {
    let mainEntryTemplate =
      makeMainTemplate(~filepath=mainEntryModulePath, ~items);
    let () = Fs.mkDirSync(entriesOutputDir, {recursive: true});
    Fs.writeFileSync(~path=mainEntryJsPath, ~data=mainEntryTemplate);
    Fs.writeFileSync(~path=mainEntryHtmlPath, ~data=htmlTemplate);
  };

  // let () = {
  //   demos->Belt.List.forEach(extractedDemo => {
  //     let finalFilepath =
  //       Path.join2(
  //         entriesOutputDir,
  //         targetPathToFilepath(extractedDemo.targetPath),
  //       );
  //     let template = makeDemoTemplate(~filepath=extractedDemo.filepath);
  //     let () = Fs.mkDirSync(Path.dirname(finalFilepath), {recursive: true});
  //     Fs.writeFileSync(~path=finalFilepath, ~data=template);
  //   });
  // };

  let mainRenderedPage: RenderedPage.t = {
    path: ["./"],
    entryPath: mainEntryJsPath,
    htmlTemplatePath: mainEntryHtmlPath,
  };

  let _ =
    Esbuild.build(
      ~outputDir=esbuildOutputDir,
      ~projectRootDir="",
      ~globalEnvValues=[||],
      ~renderedPages=[|mainRenderedPage|],
      ~logLevel=Esbuild.LogLevel.Debug,
      // ~port=8000,
      (),
    );

  ();
  // Js.log2("!!! demos:\n", Util.inspect(demos));
  // Js.log2(
  //   "!!! finalFilepaths:\n",
  //   Util.inspect(finalFilepaths->Belt.List.toArray),
  // );
};
