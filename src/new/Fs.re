[@mel.module "node:fs"]
external writeFileSync: (~path: string, ~data: string) => unit =
  "writeFileSync";

type mkDirOptions = {recursive: bool};

[@mel.module "node:fs"]
external mkDirSync: (string, mkDirOptions) => unit = "mkdirSync";
