{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      inherit (nixpkgs) lib;
      forAllSystems = lib.genAttrs lib.systems.flakeExposed;
    in
    {
      devShells = forAllSystems (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};

          qmk-firmware = pkgs.fetchFromGitHub {
            owner = "SonixQMK";
            repo = "qmk_firmware";
            rev = "b7b245d014721f727f0f8f701026939468f4c916";
            sha256 = "sha256-qTCDvXF9ZM13jyOb2k4WnlZzyPuV6mzCO7v7taxquWg=";
            fetchSubmodules = true;
          };

          sonixflasher = pkgs.stdenv.mkDerivation rec {
            pname = "sonixflasher";
            version = "2.0.8";
            src = pkgs.fetchFromGitHub {
              owner = "SonixQMK";
              repo = "SonixFlasherC";
              rev = version;
              sha256 = "sha256-dk6YFRlAz/+oZFuqJyqBmkpUvtNS+mi92cdZuK/Ke6U=";
            };
            nativeBuildInputs = with pkgs; [
              gnumake
              pkg-config
            ];
            buildInputs = with pkgs; [ hidapi ];
            buildPhase = "make";
            installPhase = ''
              mkdir -p $out/bin
              cp sonixflasher $out/bin/sonixflasher
            '';
            meta = with lib; {
              description = "CLI Flasher for Sonix SN32F2xx MCUs";
              homepage = "https://github.com/SonixQMK/SonixFlasherC";
              license = licenses.gpl3Only;
              platforms = platforms.linux;
            };
          };
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              avrdude
              clang-tools
              dfu-programmer
              dfu-util
              dos2unix
              gcc-arm-embedded
              gnumake
              pkg-config
              pkgsCross.avr.buildPackages.gcc
              qmk
              sonixflasher
              git
            ];
            buildInputs = with pkgs; [ hidapi ];
            LD_LIBRARY_PATH = lib.makeLibraryPath [ pkgs.hidapi ];

            shellHook = ''
              export QMK_HOME="$PWD/qmk_firmware"

              if [[ ! -d "$QMK_HOME" ]]; then
                mkdir -p "$QMK_HOME"

                shopt -s dotglob
                for item in "${qmk-firmware}"/*; do
                    [[ -e "$item" ]] || continue
                    basename=$(basename "$item")
                    if [[ "$basename" != "keyboards" && "$basename" != "." && "$basename" != ".." ]]; then
                        ln -s "$item" "$QMK_HOME/"
                    fi
                done
                shopt -u dotglob

                mkdir -p $QMK_HOME/keyboards/''${PWD#''${PWD%/*/*}/}
                for item in "$PWD/src"/*; do
                    ln -s "$item" "$QMK_HOME/keyboards/''${PWD#''${PWD%/*/*}/}/"
                done
              fi
            '';
          };
        }
      );
    };
}
