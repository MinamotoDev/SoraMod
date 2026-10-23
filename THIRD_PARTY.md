# Third-party resources

- **Noto Sans**: https://github.com/notofonts/noto-fonts . SIL Open Font License 1.1; see `assets/fonts/OFL-NotoSans.txt`. Bundled fallback for extended Latin characters.
- **Noto Sans CJK**: https://github.com/notofonts/noto-cjk . SIL Open Font License 1.1; license and copyright notices: `assets/fonts/OFL.txt`. Bundled original font and rasterized menu glyphs are covered by that license. No operating-system fonts are required.
- **Vehicle facts and localized display names**: https://github.com/DurtyFree/gta-v-data-dumps (`vehicles.json`, v3717.0 data snapshot). Model availability is checked in the installed game. Newer runtime DLC models absent from the catalog use a numeric identifier.
- **Native identifiers and signatures**: https://github.com/alloc8or/gta5-nativedb-data . The generated header contains only the native identifiers referenced by SoraMod.
- **Script Hook V**: https://www.dev-c.com/gtav/scripthookv/ by Alexander Blade. Runtime, ASI Loader and SDK are not included in the release package. Installed separately under their own terms.

The executable and source files authored for SoraMod are copyright (c) 2026 MinamotoDev. SoraMod code is licensed under the GNU General Public License version 3 (GPL-3.0-only); see `LICENSE`. Third-party fonts retain their own licenses. Research downloads and the local compiler are not part of the release package.

- **In-game vehicle store texture identifiers**: factual model/dictionary/texture associations cross-referenced against https://github.com/itsjustcurtis/MenyooSP/blob/Development/Solution/source/Submenus/VehicleSpawner.cpp . SoraMod uses its own drawing and input code. Images are read from the installed game; no vehicle photographs are bundled.

- **Default control associations**: https://docs.fivem.net/docs/game-references/controls/ . Factual key/button mappings supplement runtime binding checks for held input suppression.
