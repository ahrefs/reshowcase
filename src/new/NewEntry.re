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

let makeMainTemplate =
    (~filepath: string, ~items: array(NewEntity.item), ~publicPath: string) => {
  // We also call JSON.stringify below because the data interpolated to js file as a normal js object
  let itemsJsonString = items->NewEntity.items_to_json_string;
  {j|
import * as Demo from "$(filepath)";
import * as Client from "react-dom/client";
import * as JsxRuntime from "react/jsx-runtime";

const root = document.querySelector("#root");

const publicPath = "$(publicPath)";

const itemsJsonString = JSON.stringify($(itemsJsonString));

if (!(root == null)) {
  const root1 = Client.createRoot(root);
  root1.render(JsxRuntime.jsx(Demo.make,
  {
    itemsJsonString: itemsJsonString,
    publicPath: publicPath
  }));
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

let envOutputDir = Process.env->Js.Dict.get("OUTPUT_DIR");

let envDemoHtmlTemplatePath =
  Process.env->Js.Dict.get("DEMO_HTML_TEMPLATE_PATH");

let customConfigPath = Process.env->Js.Dict.get("CUSTOM_CONFIG_PATH");

let envPort =
  Process.env->Js.Dict.get("PORT")->Belt.Option.flatMap(int_of_string_opt);

let mode =
  Process.env->Js.Dict.get("MODE")->Belt.Option.getWithDefault("build");

let mode =
  switch (mode) {
  | "build" => Bundler.Build
  | "watch" => Watch
  | _ => Build
  };

let start =
    (
      ~outputDir: string,
      ~port: option(int)=?,
      ~items: array(NewEntity.item),
      ~demoHtmlTemplatePath: option(string)=?,
      (),
    ) => {
  let customConfigPromise =
    switch (customConfigPath) {
    | None => Promise.resolve(None)
    | Some(path) =>
      Esbuild.CustomConfig.readCustomConfig(~customConfigPath=path)
    };

  customConfigPromise
  ->Promise.map(customConfig => {
      let outputDir = envOutputDir->Belt.Option.getWithDefault(outputDir);
      let demos = extractDemos(~items);
      // TODO double check this
      let esbuildOutputDir = outputDir;

      let mainEntryModulePath = NewReshowcaseUi2.modulePath;
      let mainEntryJsPath = Path.join2(outputDir, "main.js");
      let mainEntryTemplate =
        makeMainTemplate(
          ~filepath=mainEntryModulePath,
          ~items,
          ~publicPath=
            customConfig
            ->Belt.Option.flatMap(config => config.publicPath)
            ->Belt.Option.getWithDefault("/"),
        );

      let mainEntry: Esbuild.Entry.t = {
        path: "/",
        entryPath: mainEntryJsPath,
      };

      let () = Fs.mkDirSync(outputDir, {recursive: true});
      let () =
        Fs.writeFileSync(~path=mainEntryJsPath, ~data=mainEntryTemplate);

      let demosEntries = {
        demos
        ->Belt.List.map(extractedDemo => {
            let demoEntryJsPath =
              Path.join2(
                outputDir,
                demoTargetPathToJsEntryPath(extractedDemo.targetPath),
              );

            let template = makeDemoTemplate(~filepath=extractedDemo.filepath);
            let () =
              Fs.mkDirSync(
                Path.dirname(demoEntryJsPath),
                {recursive: true},
              );
            let () = Fs.writeFileSync(~path=demoEntryJsPath, ~data=template);

            let demoPath = extractedDemo.targetPath->targetPathToPath;

            // Generate index.html (main app) for this demo path
            let mainAppRenderedPage: Esbuild.Entry.t = {
              path: demoPath,
              entryPath: mainEntryJsPath,
            };

            // Generate iframe.html (demo only) for this demo path
            let iframeRenderedPage: Esbuild.Entry.t = {
              path: Path.join2(demoPath, "iframe"),
              entryPath: demoEntryJsPath,
            };

            [mainAppRenderedPage, iframeRenderedPage];
          })
        ->Belt.List.flatten;
      };

      let entries =
        Belt.Array.concat([|mainEntry|], demosEntries->Array.of_list);

      let () = {
        let outputDir = esbuildOutputDir;
        let projectRootDir = "";
        let globalEnvValues = [||];
        let entries = entries;
        let logLevel = Esbuild.LogLevel.Debug;
        let port =
          switch (envPort) {
          | Some(port) => port
          | None =>
            switch (port) {
            | Some(port) => port
            | None => 8000
            }
          };

        let demoHtmlTemplatePath =
          switch (envDemoHtmlTemplatePath) {
          | Some(path) => Some(path)
          | None => demoHtmlTemplatePath
          };

        switch (mode) {
        | Build =>
          let _promise: Js.promise(unit) =
            Esbuild.build(
              ~outputDir,
              ~projectRootDir,
              ~customConfig,
              ~globalEnvValues,
              ~entries,
              ~logLevel,
              ~demoHtmlTemplatePath?,
              (),
            );
          ();
        | Watch =>
          let _promise: Js.promise(Esbuild.serveResult) =
            Esbuild.watchAndServe(
              ~outputDir,
              ~projectRootDir,
              ~customConfig,
              ~globalEnvValues,
              ~entries,
              ~logLevel,
              ~port,
              ~demoHtmlTemplatePath?,
              (),
            );
          ();
        };
      };
      ();
    })
  ->ignore;
};
