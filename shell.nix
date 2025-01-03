with import <nixpkgs> { };

mkShell {
  buildInputs = [
    xmake
    stdenv.cc
    # glibc
    # gcc14
  ];

  shellHook = ''
    LD="$CC"
    echo "Development shell initialized!"
  '';
}
