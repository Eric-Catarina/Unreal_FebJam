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

        devenvCustomDirPath = "./devenv-custom";
        projectName = "MonoGrinding";
        pwd = builtins.getEnv "PWD";
        ueBatchFilesDirPath = "${ueRootDirPath}/Engine/Build/BatchFiles/Linux";
        ueCentosBinariesDirPath = "${ueRootDirPath}/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v22_clang-16.0.6-centos7/x86_64-unknown-linux-gnu/bin";
        ueEditorBinPath = "${ueRootDirPath}/Engine/Binaries/Linux/UnrealEditor";
        ueRootDirPath = "/mnt/storage/unreal_editors/ue5.3.2";
        uprojectPath = "${pwd}/${projectName}.uproject";
      in {
        default = devenv.lib.mkShell {
          inherit inputs pkgs;
          modules = [
            {
              # https://devenv.sh/reference/options/
              packages = with pkgs; [
                clang-tools_16
                clang_16
                dotnet-sdk
                libllvm
                mach-nix-pkgs
                python3
              ];

              scripts = {
                openue.exec = ''
                  nvidia-offload steam-run ${ueEditorBinPath} ${uprojectPath}
                '';

                de-use_nix_bins.exec = ''
                  mv ${ueBatchFilesDirPath}/SetupDotnet.sh ${ueBatchFilesDirPath}/SetupDotnet.sh.bak
                  mv ${ueCentosBinariesDirPath}/clang++ ${ueCentosBinariesDirPath}/clang++.bak
                  mv ${ueCentosBinariesDirPath}/llvm-ar ${ueCentosBinariesDirPath}/llvm-ar.bak

                  cp ${devenvCustomDirPath}/SetupDotnet-Patched.sh ${ueBatchFilesDirPath}/SetupDotnet.sh
                  cp ${devenvCustomDirPath}/system_clang++_wrapper ${ueCentosBinariesDirPath}/clang++
                  cp ${devenvCustomDirPath}/system_llvm-ar_wrapper ${ueCentosBinariesDirPath}/llvm-ar
                '';

                de-use_ue_bins.exec = ''
                  mv ${ueBatchFilesDirPath}/SetupDotnet.sh.bak ${ueBatchFilesDirPath}/SetupDotnet.sh
                  mv ${ueCentosBinariesDirPath}/clang++.bak ${ueCentosBinariesDirPath}/clang++
                  mv ${ueCentosBinariesDirPath}/llvm-ar.bak ${ueCentosBinariesDirPath}/llvm-ar
                '';

                de-uecli_gen.exec = ''
                  de-use_nix_bins
                  UE_USE_SYSTEM_DOTNET=1 ${devenvCustomDirPath}/uecli_gen_wrapper.sh
                  de-use_ue_bins
                '';

                de-ubt_gen.exec = ''
                  ${ueRootDirPath}/Engine/Build/BatchFiles/Linux/Build.sh -mode=GenerateClangDatabase \
                    -project=${uprojectPath} ${projectName}Editor Development Linux -OutputDir=${pwd}
                '';

                de-debug-skipbuild.exec = ''
                  de-use_nix_bins

                  UE_USE_SYSTEM_DOTNET=1 ${ueRootDirPath}/Engine/Build/BatchFiles/Linux/Build.sh \
                    ${projectName}Editor Linux DebugGame -SkipBuild -project=${uprojectPath}

                  de-use_ue_bins
                '';

                gen.exec = ''
                  running_msg="C++ Gen Running..."
                  completed_msg="C++ Gen Completed!"

                  uecli_gen_log_path="de-uecli_gen.log"
                  skipbuild_log_path="de-debug-skipbuild.log"

                  ${pkgs.libnotify}/bin/notify-send --icon=dialog-information "${projectName}" "$running_msg"
                  echo "$running_msg"
                  de-uecli_gen &> "$uecli_gen_log_path" || (cat "$uecli_gen_log_path" | grep --color error && false)
                  de-debug-skipbuild &> "$skipbuild_log_path" || (cat "$skipbuild_log_path" | grep --color error && false)
                  echo "$completed_msg"
                  ${pkgs.libnotify}/bin/notify-send --icon=face-smile "${projectName}" "$completed_msg"
                '';
              };
            }
          ];
        };
      });
  };
}
