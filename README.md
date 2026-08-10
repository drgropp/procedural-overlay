# Procedural Overlay

Procedural Overlay is a small C11/raylib program for Windows that displays
animated procedural graphics in a borderless, resizable, always-on-top window.
It has no installer; build it and run the generated executable directly.

## Visual modes

- **Sphere** - a rotating, rainbow-colored point sphere.
- **Vortex** - nested, spinning hexagonal rings.
- **Waves** - layered animated sine waves.
- **Ambience** - slowly drifting colored lights and glows.
- **Spectrum** - animated rainbow bars with reflections.

## Requirements

- Windows
- [w64devkit](https://github.com/skeeto/w64devkit), providing GCC and
  `mingw32-make`
- CMake
- A local raylib source tree
- Git, only if you need to clone the repository

The current `build.bat` is machine-specific: it sets `TOOLS` to
`C:\raylib\w64devkit\bin` and expects `cmake.exe`, `gcc.exe`, and
`mingw32-make.exe` in that directory. If your toolchain is elsewhere, update
the `TOOLS` line before building. For example, a verified Windows setup uses
`C:\w64devkit\bin`.

raylib does **not** need to be installed at `C:\raylib\raylib`. That path is
only the current default in `CMakeLists.txt`. `RAYLIB_ROOT` is a CMake cache
variable and should point to the raylib source directory (the directory that
contains raylib's `CMakeLists.txt`).

## Build and run

If you do not already have the repository:

```powershell
git clone https://github.com/drgropp/procedural-overlay.git
cd procedural-overlay
```

From the `procedural-overlay` repository, configure a non-default raylib source
location once before the first build. This example matches a toolchain at
`C:\w64devkit`; replace the raylib path with your own:

```powershell
C:/w64devkit/bin/cmake.exe -S . -B build -G "MinGW Makefiles" -DRAYLIB_ROOT="D:/path/to/raylib" -DCMAKE_MAKE_PROGRAM="C:/w64devkit/bin/mingw32-make.exe" -DCMAKE_C_COMPILER="C:/w64devkit/bin/gcc.exe"
```

Also update `TOOLS` in `build.bat` to `C:\w64devkit\bin` for that layout. The
configured `RAYLIB_ROOT` is then retained in `build/CMakeCache.txt`.

Build from the repository root:

```powershell
cmd.exe /c build.bat
```

Run:

```powershell
./build/procedural_overlay.exe
```

The executable is produced at `build/procedural_overlay.exe`.

## Controls

- Press **Left Arrow** or **Right Arrow** to switch visual modes.
- Click the **<** or **>** buttons to switch modes when the controls are shown.
- Press **H** or **F11** to show or hide the controls.
- Type `:` to open command entry, type a command, and press **Enter** to run it.
- Press **Backspace** while entering a command to correct it.
- Drag the top strip to move the window.
- Drag an edge or corner to resize the window.
- Press **Esc** or **Alt+F4** to exit.

### Color commands

Color commands change the current visual immediately without restarting it or
resetting its animation:

- `:red`, `:orange`, `:yellow`, `:green`, `:blue`, `:indigo`, and
  `:violet` select shaded versions of the original color set.
- `:cyan` selects a bright cyan/terminal-blue palette.
- `:magenta` selects a vivid magenta palette.
- `:white` selects a luminous white and soft-gray palette.
- `:amber` selects a warm amber/orange CRT-style palette.
- `:pink` selects a pink palette.
- `:terminal` uses dark terminal greens with bright green highlights.
- `:ice` uses related pale blue and cyan shades.
- `:fire` distributes animated red, orange, and yellow shades.
- `:rainbow` is the default and restores the original multicolor behavior.
- `:color` reports the currently active palette without changing it.
- `:help` shows all palette commands in a compact temporary overlay.

Commands are normalized to lowercase as they are typed. An unrecognized
command displays a short message, such as `Unknown command: :purplee`, without
interrupting the current visual. Selecting a palette briefly displays its name,
for example `Palette: Terminal`.

Non-rainbow palettes reuse each effect's existing color inputs to produce
lighter and darker shades while retaining the original alpha and glow layers.
Terminal, ice, and fire additionally map those inputs across a narrow related
hue range. Rainbow bypasses this mapping so its original appearance is
unchanged.

## Project structure

```text
procedural-overlay/
|-- build.bat                 Windows configure/build script
|-- CMakeLists.txt            CMake project and RAYLIB_ROOT setting
|-- src/
|   |-- main.c                Window, input, and mode selection
|   |-- effects.c             Procedural effect implementations
|   `-- effects.h             Effect interface
|-- tests/
|   `-- palette_tests.c       Palette command and color-mapping tests
`-- build/                    Generated build files (created locally)
    `-- procedural_overlay.exe
```

The `build/` directory is generated and ignored by Git. The external raylib
source tree remains at the location referenced by `RAYLIB_ROOT`; its generated
build files appear under `build/raylib/`.

## Troubleshooting

- **Toolchain not found:** check the `TOOLS` value in `build.bat`. It must name
  the directory containing `cmake.exe`, `gcc.exe`, and `mingw32-make.exe`.
- **raylib source not found:** configure `RAYLIB_ROOT` with the real raylib
  source path. `C:\raylib\raylib` is only the project default, not a
  requirement.
- **CMake uses an old path or generator:** CMake stores toolchain and raylib
  paths in `build/CMakeCache.txt`; changing an environment variable alone does
  not replace cached values.
- **Paths changed:** delete the generated `build/` directory, repeat the
  one-time `RAYLIB_ROOT` configuration above, and run
  `cmd.exe /c build.bat` again.
