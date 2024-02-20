{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    systems.url = "github:nix-systems/default";
    devenv.url = "github:cachix/devenv";
    mach-nix.url = "mach-nix/3.5.0";
  };

  nixConfig = {
    extra-trusted-public-keys = "devenv.cachix.org-1:w1cLUi8dv3hnoSPGAuibQv+f9TZLr6cv/Hm9XgU50cw=";
    extra-substituters = "https://devenv.cachix.org";
  };

  outputs = {
    self,
    nixpkgs,
    devenv,
    systems,
    ...
  } @ inputs: let
    l = inputs.nixpkgs.lib // builtins;
    supportedSystems = ["x86_64-linux" "aarch64-darwin"];
    forEachSystem = f:
      l.genAttrs supportedSystems
      (system: f system (import inputs.nixpkgs {inherit system;}));
  in {
    packages = forEachSystem (system: pkgs: {
      devenv-up = self.devShells.${system}.default.config.procfileScript;
    });

    devShells =
      forEachSystem
      (system: pkgs: let
        pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true;
        };

        mach-nix-pkgs = inputs.mach-nix.lib."${system}".mkPython {
          requirements = ''
            ue4cli
          '';
        };
      in {
        default = devenv.lib.mkShell {
          inherit inputs pkgs;
          modules = [
            {
              # https://devenv.sh/reference/options/
              packages = [
                # pkgs.ccls
                mach-nix-pkgs
                pkgs.clang-tools_16
                pkgs.libllvm
                pkgs.python3
              ];

              scripts.devenv-gen-uecli.exec = ''
                nix-shell -p clang_16 dotnet-sdk --command ./scripts/clangdb_gen_uecli.sh
              '';

              scripts.devenv-gen-buildsh.exec = ''
                nix-shell -p clang_16 dotnet-sdk --command ./scripts/clangdb_gen_buildsh.sh
              '';

              scripts.devenv-debug-skipbuild.exec = ''
                nix-shell -p clang_16 dotnet-sdk --command ./scripts/clangdb_gen_debug_skip_build.sh
              '';
            }
          ];
        };
      });
  };
}
