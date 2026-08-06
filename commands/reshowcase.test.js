const assert = require("node:assert/strict");
const { spawn, spawnSync } = require("node:child_process");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const test = require("node:test");

const cliPath = path.join(__dirname, "reshowcase");

const stopServer = (server) =>
  new Promise((resolve) => {
    if (server.exitCode !== null) {
      resolve();
      return;
    }
    server.once("exit", resolve);
    server.kill("SIGTERM");
  });

const fetchHtml = async (url, server) => {
  for (let attempt = 0; attempt < 50; attempt++) {
    if (server.exitCode !== null) {
      throw new Error(`reshowcase exited with code ${server.exitCode}`);
    }
    try {
      const response = await fetch(url);
      if (response.ok) {
        return response.text();
      }
    } catch {}
    await new Promise((resolve) => setTimeout(resolve, 100));
  }
  throw new Error("reshowcase did not start within 5 seconds");
};

test("only enables live reload in watch mode", async (t) => {
  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), "reshowcase-test-"));
  const entryPath = path.join(tempDir, "entry.js");
  const templatePath = path.join(tempDir, "template.html");
  const outputPath = path.join(tempDir, "build");
  fs.writeFileSync(
    entryPath,
    'document.getElementById("root").textContent = "Reshowcase";'
  );
  fs.writeFileSync(
    templatePath,
    '<!doctype html><html><head></head><body><div id="root"></div></body></html>'
  );
  t.after(() => fs.rmSync(tempDir, { recursive: true, force: true }));

  const build = spawnSync(
    process.execPath,
    [
      cliPath,
      "build",
      `--entry=${entryPath}`,
      `--template=${templatePath}`,
      `--output=${outputPath}`,
    ],
    { encoding: "utf8" }
  );
  assert.equal(build.status, 0, build.stderr || build.stdout);
  const builtHtml = fs.readFileSync(path.join(outputPath, "index.html"), "utf8");
  assert.doesNotMatch(builtHtml, /EventSource|\/esbuild/);

  const port = 20000 + (process.pid % 10000);
  const server = spawn(
    process.execPath,
    [
      cliPath,
      "start",
      `--entry=${entryPath}`,
      `--template=${templatePath}`,
      `--port=${port}`,
    ],
    { env: { ...process.env, TMPDIR: tempDir }, stdio: "ignore" }
  );
  t.after(() => stopServer(server));

  const watchedHtml = await fetchHtml(`http://127.0.0.1:${port}/`, server);
  assert.match(watchedHtml, /new EventSource\('\/esbuild'\)/);
});
