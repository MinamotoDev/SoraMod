# SoraMod

[English](README.md) · [Français](docs/README.fr.md) · [Italiano](docs/README.it.md) · [Deutsch](docs/README.de.md) · [Español (España)](docs/README.es-es.md) · [日本語](docs/README.ja.md) · [한국어](docs/README.ko.md) · [Polski](docs/README.pl.md) · [Português (Brasil)](docs/README.pt-br.md) · [Русский](docs/README.ru.md) · [简体中文](docs/README.zh-cn.md) · [Español (Latinoamérica)](docs/README.es-419.md) · [正體中文](docs/README.zh-tw.md)

A Story Mode trainer for GTA V Enhanced by MinamotoDev. Supports keyboard, mouse and controller, with bundled fonts for the menu.

[Download latest version](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Release notes](RELEASE_NOTES.md)

## Features

- Invincibility, health and armor restoration, wanted level removal, ammunition refill and vehicle repair.
- Spawn vehicles by category, full-screen cards, random selection or model name, including Online-exclusive models available in your game.
- Pin vehicles and save their position, plate, colors and common modifications; remotely unpin them, teleport into them or mark them on the map.
- Teleport to a map waypoint; change the time, freeze the clock or switch the weather.
- The menu follows the game language.

## Installation

1. Add `-nobattleye` to `args.txt` in the game directory.
2. Install [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) and an ASI Loader compatible with your game version.
3. Download `SoraMod-0.4.1-win-x64.zip` and copy `SoraMod.asi` and the `SoraMod` folder into the game directory.
4. Enter Story Mode and press `F7` to open the menu.

Requires 64-bit Windows 10 or later and GTA V Enhanced. Story Mode only. Exit the game before updating and overwrite the mod files; `SoraMod/settings.ini` and `SoraMod/data` store settings and saved data.

## Controls

| Action | Keyboard | Controller |
| --- | --- | --- |
| Open / hide | F7 | Hold RB / R1, then D-pad Right |
| Select an item in a list | Up / Down | D-pad Up / Down |
| Next list page | Right / Page Down | D-pad Right / RB |
| Previous list page; back at the first page | Left / Backspace | D-pad Left |
| Move between full-screen cards | Arrow keys: up/down by row, left/right to adjacent cards | D-pad Up / Down / Left / Right |
| Confirm | Enter | A / Cross |
| Parent menu; return to the game at the root | Esc | B / Circle |
| Full-screen category tabs | Tab / Shift+Tab | RB / LB (R1 / L1) |
| Previous page directly | Page Up | LB / L1 in regular lists |

## Vehicle data

“Spawn by model name” accepts model codes such as `adder` and `oppressor2`. The input starts blank. Available models depend on your installed game version.

Enter “Pinned vehicles” while inside a vehicle to pin or unpin it. Records are stored in `SoraMod/data/pinned-vehicles.dat`. Position and modifications are saved every 5 seconds; pinning and unpinning save immediately. After restarting the game, vehicles are restored when you approach their saved location or choose to teleport into them. The adjacent `.bak` file is used for corruption recovery. Clearing pins releases them without deleting vehicles from the scene.

## Uninstall and feedback

Delete `SoraMod.asi` and the `SoraMod` folder to uninstall. Back up `SoraMod/data` first if you plan to use the mod again. For bug reports, include your game version, Script Hook V version, model code and `SoraMod/SoraMod.log`.

## Build

Run `tools/build.ps1 -Cxx <path to g++.exe>` with x64 MinGW-w64 GCC (C++17). Regenerating glyphs requires Python, Pillow, fontTools, the font files, `research/vehicles.json` and `research/natives.json`; run `tools/build-assets.py`. Offline tests start at `tests/run.ps1`; configure the tool paths in the scripts. See [THIRD_PARTY.md](THIRD_PARTY.md) for resource sources.

## License

SoraMod is licensed under [GNU GPL v3 (GPL-3.0-only)](LICENSE). Copyright © 2026 MinamotoDev. Bundled fonts retain their own licenses; see [THIRD_PARTY.md](THIRD_PARTY.md).
