{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
  inputs.utils.url = "github:numtide/flake-utils";
  outputs = {
    nixpkgs,
    utils,
    ...
  }:
    utils.lib.eachSystem utils.lib.allSystems (system: rec {
      packages.default = packages.p1-minefield-pathfinding;
      packages.p1-minefield-pathfinding = nixpkgs.legacyPackages.${system}.callPackage ({
        clangStdenv,
        cmake,
        fetchFromGitHub,
      }: let
        # Submodules do not get fetched by Nix Flakes when src is a path. Link our
        # dependencies manually instead.
        mtest = fetchFromGitHub {
          owner = "MortenSchou";
          repo = "mtest";
          rev = "fb0c03d5ff553b8662c7f1bc88aa7fe3bfaac823";
          hash = "sha256-8FOg+k9CH0DZp+TfBDzmhzSbLHZpg8TCRHRlIfKgfi8=";
        };
      in
        clangStdenv.mkDerivation {
          name = "p1-minefield-pathfinding";
          src = ./.;
          nativeBuildInputs = [cmake];
          preConfigure = ''
            # Workaround: https://github.com/NixOS/nix/issues/8442
            [ -d "extern/mtest" ] && rm -r extern/mtest
            ln -s ${mtest} extern/mtest
          '';
          doCheck = true;
          env.NIX_CFLAGS_COMPILE = "-O0 -Wall -Wextra -Wpedantic";
          meta.mainProgram = "main";
        }) {};
    });
}
