{
  lib,
  stdenv,
  cmake,
  boost,
}:

let
  cmakeLists = builtins.readFile ../CMakeLists.txt;
  versionMatch = builtins.match ".*project\\(lolcat VERSION ([0-9.]+)\\).*" cmakeLists;
  version =
    if versionMatch == null then
      throw "Could not parse lolcat version from CMakeLists.txt"
    else
      builtins.head versionMatch;
in
stdenv.mkDerivation (finalAttrs: {
  pname = "lolcat++";
  inherit version;

  src = ../.;

  nativeBuildInputs = [ cmake ];
  buildInputs = [ boost ];

  meta = with lib; {
    description = "Fast C++ rewrite of lolcat";
    homepage = "https://github.com/lolcatpp/lolcatpp";
    license = licenses.bsd3;
    mainProgram = "lolcat";
    platforms = platforms.unix;
  };
})
