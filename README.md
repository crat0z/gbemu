# gbemu
another Gameboy emulator written in C++, comes with a debugger, see preview

# preview
![debugger](https://i.imgur.com/2kGX79f.png)
![example](https://i.imgur.com/kcRYbrD.png)
![all](https://i.imgur.com/q591hi0.png)

# plans
- [x] unspaghettify, in particular make everything more modular for future emulators
- [x] stack viewer
- [x] memory viewer
- [x] unify stack/memory/register/disassembler windows, so e.g. open xxxx address in disassembler to view in memory viewer, view I in memory viewer, etc.
- [ ] call graph drawing, but this one might take a lot of work
- [ ] implement SCHIP etc quirks and settings to enable/disable
- [ ] actually fix the GUI in some spots

# how to build??

## windows
use VS and open folder as a cmake project. press build. just works

## linux
you need some packages, but i forget what. mostly xorg devel packages, but thats about it. gcc and clang are both fine

## otherwise
gl
