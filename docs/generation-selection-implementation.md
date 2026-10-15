# Generation selection — implementation plan (phase 1)

Status: architecture documented; gameplay integration NOT yet implemented.

## Player-facing choices
- Generation: 1–9, or All.
- Mode: ONLY (introduced in selected generation) or UNLOCKED (introduced in or before selected generation).
- All ignores mode and allows all enabled generations.
- New Game only; selection persists per save. Existing saves must default to All until a migration strategy is verified.

## Core API (proposed)
```c
enum GenerationMode { GEN_MODE_ONLY, GEN_MODE_UNLOCKED };
struct GenerationSelection { u8 generation; u8 mode; };
bool8 IsSpeciesAllowedByGeneration(u16 species);
u8 GetSpeciesIntroductionGeneration(u16 species);
```
Centralize validation and compatibility logic. Do NOT use compile-time P_GEN_N_POKEMON switches as runtime settings: those are build-time toggles. Do NOT allocate one event flag per species or trainer.

## Integration points
- New Game UI: src/main_menu.c transitions to Oak speech; add a generation/mode screen before beginning the introduction, retaining B-button cancellation.
- Save: inspect existing SaveBlock layout and reserved space before allocating persistent fields; initialize defaults on New Game and migrate old saves safely.
- Wild: hook encounter table selection/weighted slot selection, preserving area, time, method, level and slot weights. Avoid rejection loops.
- Trainers: select deterministic curated parties for bosses; filtered, role-aware curated pools for ordinary trainers; maintain team size, level, moves, held items and AI.
- Starters and static encounters: use explicit curated lists and special-case policies, never silently substitute arbitrary species.
- Regional forms and cross-generation evolutions: define an explicit generation attribution policy, with tests.

## Required validation before gameplay code
1. Confirm exact save layout and spare bytes; old-save behavior.
2. Confirm trainer-party loader and encounter generation entry points.
3. Confirm starter script modifications and New Game transitions.
4. Add tests for generations 1–9, Only, Unlocked, All, cross-gen evolutions and forms.
5. Compile FRLG target and test save/load and representative battles/encounters.

## Patch notes
- Development: started generation-selection implementation planning and isolated feature branch; no gameplay behavior changed.

## Implementation progress (2026-10-15)
- Added `include/generation_selection.h` and `src/generation_selection.c`.
- Reserved `VAR_GENERATION_SELECTION` (0x40F9) as one persistent packed configuration value.
- Implemented getters, setter, National Dex introduction-generation lookup, and species eligibility checks.
- Unset/legacy values default to All Generations. Unknown species and non-Dex forms are rejected by eligibility.
- Makefile discovers `src/*.c` automatically; no build manifest edit required.
- **Not yet connected** to New Game UI, encounters, trainer parties, starter events, or existing save migration tests.
- **Build and runtime testing not yet performed.**
- National Dex generation policy applies to species' base Pokédex number; regional forms need a separate policy.

- Added staging API for generation choice before New Game; `NewGameInitData` applies it after `InitEventData` clears event variables. Menu still needs to call the staging API.

- Implemented first-pass Oak intro generation chooser in `src/oak_speech.c`: after Oak's closing dialogue, player chooses All or Gen 1–9; specific generations then select Only/Unlocked. Choice is staged for New Game initialization. This is uncompiled/unverified UI code and may need layout/text/flow corrections after testing.

## Bedroom Adventure Settings revision (2026-10-15)
- Restored the vanilla Oak introduction; removed the temporary generation menu and its input handlers from `src/oak_speech.c`.
- Repurposed the existing bedroom notice interaction in `data/maps/PalletTown_PlayersHouse_2F_Frlg/scripts.inc` to introduce Adventure Settings and distinguish permanent generation choices from later-adjustable options such as EXP sharing.
- Retained generation selection eligibility and persistent configuration code for reuse in the bedroom menu.
- **Still pending:** interactable settings selection UI, one-time staircase confirmation with bypass-proof transition handling, separate in-game EXP Share controls, encounter/trainer/starter filtering, build and emulator tests.
