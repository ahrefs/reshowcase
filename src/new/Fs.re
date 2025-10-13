[@mel.module "node:fs"]
external writeFileSync: (~path: string, ~data: string) => unit =
  "writeFileSync";
