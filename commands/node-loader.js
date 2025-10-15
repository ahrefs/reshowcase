import { fileURLToPath } from "url";
import { basename } from "path";

function getFilenameWithoutExtension(filePath) {
  return basename(filePath, ".js"); // or use extname to auto-detect
}

export async function load(url, context, nextLoad) {
  if (url.includes("_Doc")) {
    const filePath = fileURLToPath(url);

    const filename = getFilenameWithoutExtension(filePath);

    const stubModule = `
      export const modulePath = "${filePath}";
      export const demoName = "${filename}";
    `;

    return {
      format: "module",
      source: stubModule,
      shortCircuit: true,
    };
  }

  return nextLoad(url, context);
}
