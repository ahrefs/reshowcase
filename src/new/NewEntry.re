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
  // We also call JSON.stringify below because the data interpolated to js file as a normal js object
  let itemsJsonString = items->NewEntity.items_to_json_string;
  {j|
import * as Demo from "$(filepath)";
import * as Client from "react-dom/client";
import * as JsxRuntime from "react/jsx-runtime";

const root = document.querySelector("#root");

const itemsJsonString = JSON.stringify($(itemsJsonString));

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
  // original path to the compiled demo module
  filepath: string,
  // path segments according to the structure defined by user (category names + demo name as the last segment)
  targetPath: list(string),
};

let targetPathToPath = targetPath => {
  targetPath
  ->List.rev
  ->Belt.List.map(Utils.slugify)
  ->Belt.List.toArray
  ->Js.Array.join(~sep="/", _);
};

let demoTargetPathToJsEntryPath = targetPath => {
  let path = targetPathToPath(targetPath);
  Path.join2(path, "demo.js");
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
  let demos = extractDemos(~items);
  // Js.log2("!!! extracted demos:\n", Util.inspect(demos->Array.of_list));

  let mainEntryModulePath = NewReshowcaseUi2.modulePath;
  let mainEntryJsPath = Path.join2(entriesOutputDir, "main.js");
  let mainEntryTemplate =
    makeMainTemplate(~filepath=mainEntryModulePath, ~items);

  let mainRenderedPageRoot: RenderedPage.t = {
    path: "/",
    entryPath: mainEntryJsPath,
  };

  let () = Fs.mkDirSync(entriesOutputDir, {recursive: true});
  let () = Fs.writeFileSync(~path=mainEntryJsPath, ~data=mainEntryTemplate);

  let demosRenderedPages = {
    demos
    ->Belt.List.map(extractedDemo => {
        let demoEntryJsPath =
          Path.join2(
            entriesOutputDir,
            demoTargetPathToJsEntryPath(extractedDemo.targetPath),
          );
        let template = makeDemoTemplate(~filepath=extractedDemo.filepath);
        let () =
          Fs.mkDirSync(Path.dirname(demoEntryJsPath), {recursive: true});
        let () = Fs.writeFileSync(~path=demoEntryJsPath, ~data=template);

        let demoPath = extractedDemo.targetPath->targetPathToPath;

        // Generate index.html (main app) for this demo path
        let mainAppRenderedPage: RenderedPage.t = {
          path: demoPath,
          entryPath: mainEntryJsPath,
        };

        // Generate iframe.html (demo only) for this demo path
        let iframeRenderedPage: RenderedPage.t = {
          path: Path.join2(demoPath, "iframe"),
          entryPath: demoEntryJsPath,
        };

        [mainAppRenderedPage, iframeRenderedPage];
      })
    ->Belt.List.flatten;
  };

  let renderedPages =
    Belt.Array.concat(
      [|mainRenderedPageRoot|],
      demosRenderedPages->Array.of_list,
    );

  // let _ =
  //   Esbuild.build(
  //     ~outputDir=esbuildOutputDir,
  //     ~projectRootDir="",
  //     ~globalEnvValues=[||],
  //     ~renderedPages,
  //     ~logLevel=Esbuild.LogLevel.Debug,
  //     // ~port=8000,
  //     (),
  //   );

  let _ =
    Esbuild.watchAndServe(
      ~outputDir=esbuildOutputDir,
      ~projectRootDir="",
      ~globalEnvValues=[||],
      ~renderedPages,
      ~logLevel=Esbuild.LogLevel.Debug,
      ~port=8000,
      (),
    );

  ();
  // Js.log2("!!! demos:\n", Util.inspect(demos));
  // Js.log2(
  //   "!!! finalFilepaths:\n",
  //   Util.inspect(finalFilepaths->Belt.List.toArray),
  // );
};
