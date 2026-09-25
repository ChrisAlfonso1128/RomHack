# RomHack source map

This is a quick orientation guide for Mike and Chris. Exact implementation details can vary by feature, so use the changelog **Change trace** for the precise file/line/anchor used by a specific change.

## Trainers

Common files:

- `src/data/trainers.party`
- `src/data/trainers_frlg.party`
- `include/constants/trainers.h`

For FireRed trainer-party work, check the FRLG-specific trainer data first, then follow the trainer constant/anchor recorded in the changelog.

## Pokémon species and forms

Common files/directories:

- `include/constants/species.h`
- `include/config/species_enabled.h`
- `src/data/pokemon/species_info.h`
- `src/data/pokemon/species_info/`
- `src/data/pokemon/level_up_learnsets/`
- `src/data/pokemon/form_change_tables.h`
- `src/data/pokemon/form_species_tables.h`

These areas cover species IDs, enabled generations/families, species data, learnsets, and form/evolution-related structures.

## Wild encounters

Common files:

- `src/data/wild_encounters.json`
- `src/data/wild_encounters.constants.json.txt`
- `include/config/wild_encounter.h`

## Items

Common files:

- `include/constants/items.h`
- `src/data/items.h`
- `include/config/item.h`

Item behavior may also lead into C source depending on what the item does.

## Maps and local map scripts

Each map generally lives under:

- `data/maps/<MapName>/map.json`
- `data/maps/<MapName>/scripts.inc`

Use **Porymap** for visual tile/layout work. The JSON/script files are useful for events, object metadata, warps, scripts, and troubleshooting.

Because the expansion supports multiple game targets, some shared map/script content coexists with FRLG-specific content. Always verify the active FireRed path before editing.

## Shared/global scripts

Common locations:

- `data/scripts/`
- `data/scripts/*_frlg.inc`

Examples present in the repo include FRLG-specific item-ball, daycare, Pokémon Center, move-tutor, trainer-card, Fame Checker, and Hall of Fame scripts.

## Flags and variables

Common files:

- `include/constants/flags.h`
- `include/constants/flags_frlg.h`
- `include/constants/vars.h`
- `include/constants/vars_frlg.h`

For FireRed story/event work, check the FRLG-specific constants where applicable.

## Battle mechanics

Common areas:

- `src/battle_*.c`
- `src/battle_gimmick.c`
- `src/battle_script_commands.c`
- `include/config/battle.h`

Modern battle features in RHH Expansion can touch several files, so rely heavily on changelog anchors for custom mechanics.

## Pokémon engine behavior

Common areas:

- `src/pokemon.c`
- `src/pokemon_*.c`
- `include/config/pokemon.h`

## Project configuration

The expansion exposes many switches under:

- `include/config/`

Notable files include:

- `battle.h`
- `pokemon.h`
- `species_enabled.h`
- `item.h`
- `overworld.h`
- `save.h`
- `debug.h`
- `quickstart.h`

## How to troubleshoot from a changelog entry

1. Open the listed **File**.
2. Jump to the listed **Lines**.
3. Search for the **Anchor** if the line numbers have drifted.
4. Inspect any additional files listed in the same change trace.
5. Use the referenced commit/PR to see the exact diff that introduced the behavior.
6. If the issue is visual or event-order related, reproduce it locally in mGBA/Porymap and report the exact observed behavior.

The changelog is intentionally designed to teach the project structure over time: repeated changes should gradually make the important areas of the codebase familiar.
