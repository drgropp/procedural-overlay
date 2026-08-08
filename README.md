# Procedural Overlay

A small Windows desktop overlay built with C11 and raylib. It displays several
procedural visual effects in a borderless, resizable, always-on-top window.

## Requirements

- Windows
- Git
- raylib, including its bundled `w64devkit` toolchain

The included build script expects this layout:

```text
C:\raylib\raylib
C:\raylib\w64devkit\bin
```

The `bin` directory must contain `cmake.exe`, `gcc.exe`, and
`mingw32-make.exe`.

## Clone and build

```powershell
git clone https://github.com/drgropp/procedural-overlay.git
cd procedural-overlay
.\build.bat
```

The executable will be created at:

```text
build\procedural_overlay.exe
```

Run it with:

```powershell
.\build\procedural_overlay.exe
```

There is no installer; the executable can be run directly.

## Custom raylib location

If raylib or `w64devkit` is installed elsewhere, either update the paths in
`build.bat` or configure the project manually:

```powershell
cmake -S . -B build -G "MinGW Makefiles" `
  -DRAYLIB_ROOT="C:/path/to/raylib" `
  -DCMAKE_MAKE_PROGRAM="C:/path/to/w64devkit/bin/mingw32-make.exe" `
  -DCMAKE_C_COMPILER="C:/path/to/w64devkit/bin/gcc.exe"
cmake --build build
```

## Controls

- Left and right arrow keys: change the visual effect
- `H` or `F11`: show or hide the controls
- Drag the top bar: move the window
- Drag a window edge or corner: resize the window
