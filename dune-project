(lang dune 3.16)

(using melange 0.1)

(generate_opam_files true)

(name reshowcase)

(source
 (github ahrefs/reshowcase))

(documentation "https://github.com/ahrefs/reshowcase")

(bug_reports "https://github.com/ahrefs/reshowcase/issues")

(license "MIT")

(package
 (name reshowcase)
 (synopsis "A tool to create demos for your ReasonReact components")
 (description "")
 (maintainers "Denis Strelkov <denis.strelkov@ahrefs.com>")
 (authors "bloodyowl <mlbli@me.com>")
 (license "MIT")
 (tags
  ("melange" "org:ahrefs"))
 (depends
  ocaml
  (melange
   (>= 5.0.1))
  (reason
   (>= 3.11.0))
  (reason-react
   (>= 0.16.0))
  (reason-react-ppx
   (>= 0.16.0))
  (server-reason-react
   (>= 0.4.1))
  (styled-ppx
   (>= 0.60.4))
  (ocamlformat
   (>= 0.27.0))
  (ocaml-lsp-server :with-test)
  (opam-check-npm-deps :with-test) ; todo: use with-dev-setup once opam 2.2 is out
  ))
