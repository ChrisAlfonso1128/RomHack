# Development tools

RHH Expansion already includes a built-in debug menu in normal development builds.

## Opening the overworld debug menu

While in the overworld:

1. Hold **R**.
2. Press **START**.

The menu is controlled by `DEBUG_OVERWORLD_MENU` in `include/config/debug.h`. The project currently uses `DISABLED_ON_RELEASE`, which means the debug menu is enabled in normal development builds such as `make firered`, but automatically disabled in release builds.

## Useful starter-testing tools

For starter balance testing, the most useful built-in options are:

- **Give X... -> Give item XYZ...** — give items such as Rare Candy.
- **Give X... -> Pokémon (Basic/Complex)** — add test Pokémon directly.
- **Party... -> Move Relearner** — check or restore level-up moves.
- **Party... -> Heal party** — heal without returning to a Pokémon Center.
- **Party... -> Check EVs / Check IVs** — inspect test Pokémon.
- **Party... -> Set Party** — configure a test party.
- **Trainers...** — launch or configure trainer tests.
- **Utilities... -> Fly to map / Warp to map warp** — jump around the game quickly.
- **Flags & Vars...** — manipulate progression flags/variables when testing events.

## Release behavior

Do not change the debug configuration to permanently expose these tools in release builds. Use the normal development build for testing and `make release` when a public/release build should omit the debug menu.
