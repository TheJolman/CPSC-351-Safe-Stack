# c dev environment
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    clang
    lldb
    gcc
    valgrind
    gnumake
    bear
  ];
}
