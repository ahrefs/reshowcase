(lang dune 3.8)

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
   (>= 3.0.0))
  (reason
   (>= 3.10.0))
  (reason-react
   (>= 0.14.0))
  (reason-react-ppx
   (>= 0.14.0))
  (ocaml-lsp-server :with-test)
  (opam-check-npm-deps :with-test) ; todo: use with-dev-setup once opam 2.2 is out
  ))
