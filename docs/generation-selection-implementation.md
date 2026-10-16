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

- Added Adventure Settings multichoice definitions (`include/constants/script_menu.h`, `src/data/script_menu.h`) and a first-pass interactive generation chooser in the bedroom notice script. The chooser supports generations 1–9, All, and Only/Unlocked, writing the existing packed event var.
- This initial menu is **not yet locked after leaving**; staircase interception, option status display, actual encounter/trainer filtering, and compile/emulator validation remain pending. No build is claimed.

- Added `VAR_ADVENTURE_SETTINGS_CONFIRMED` (`0x40FA`) and a bedroom coordinate trigger at (10,3), directly before the upstairs warp. First visit prompts for confirmation; Yes sets the lock; No moves the player one tile away. Generation menu refuses further edits once confirmed.
- Corrected escaped paragraph/newline sequences in the Adventure Settings introduction text.
- **Verification pending:** coordinate trigger coverage and collision/path bypass checks in emulator; event script compilation; generation filtering of starters/wild/trainer parties; settings status readout; future EXP Share access. No playable build validated.

- Began runtime wild encounter filtering in `src/wild_encounter.c`: standard land/water/rock encounters prefer eligible species already in the current area's encounter slots. This is a provisional fallback, **not** full generation compliance: if no eligible species exists, the original slot remains; fishing, static, scripted encounters, and trainer teams remain to be integrated. Encounter weighting and area curation require further work.
- Build and gameplay verification are still outstanding; do not publish as tested.

- Extended the provisional encounter slot eligibility check to fishing (`src/wild_encounter.c`). Fishing currently uses the full fishing slot pool as fallback; rod-specific encounter slot weighting and level distribution require correction before release.
- Checked GitHub Actions workflow runs for commit `6042463994e74af9def5744fce131d72096670a4`: no PR-triggered workflow runs were returned. This does not establish compile success.

- Corrected fishing fallback to respect rod-specific encounter slots: Old Rod 0–1, Good Rod 2–4, Super Rod 5–9. Previously fallback could pull species from a different rod tier. Areas without eligible slots still retain the original species; weighted selection and full generation compliance remain pending.
- Inspected `CreateNPCTrainerPartyFromTrainer` in `src/battle_setup.c`: trainer party creation uses a generator and party-pool selection. Trainer generation substitution must integrate with that system rather than blindly overwrite generated Pokémon (which would lose custom moves/held items). No trainer substitution has been committed yet.

- Added a first-pass generation-eligible authored-mon substitution in `CreateNPCTrainerPartyFromTrainer` for opponent parties only. It searches the trainer's existing pool/party for an allowed species and generates that authored entry, preserving the entry's custom battle configuration. Player-side generated partner parties are not modified.
- **Limitations:** eligible fallback currently chooses the first eligible authored slot and may repeat it across party slots; when no eligible authored mon exists the original is retained. This is not the promised distinct generation-specific trainer roster. Pool eligibility, duplicate avoidance, curated rival/Gym/Elite Four teams, and build tests are pending.

- Improved provisional opposing-trainer substitution to avoid reusing an authored pool index when another eligible, unused index exists. Previously every disallowed slot selected the first eligible index. If eligible entries are fewer than the requested party size, repetition remains a deliberate fallback; fully curated per-generation teams are still required.
- Compilation, runtime tests, and verification of party-pool edge cases remain outstanding.

- Corrected bedroom confirmation to branch on symbolic `NO` and normalized event-dialogue newline/paragraph escapes in the staircase prompt.
- **Pre-test blockers:** Generation-only modes can still encounter disallowed species when an area/trainer lacks eligible authored entries. Generation-specific rival/Gym/Elite Four rosters, starter and static encounter coverage, EXP Share behavior, and compile validation remain incomplete. The code is not ready to certify for physical testing.

## Approved boss-team design policy (2026-10-15)
- Gym Leaders and Elite Four retain their established type specialty across all generation settings.
- `Generation Only`: use exclusively species introduced in the selected generation; `Generation Unlocked`: use species introduced in that generation or any earlier generation; `All`: unrestricted species pool.
- Match intended encounter difficulty using levels, moves, abilities, items, evolution stage, and team size while respecting the selected generation and specialist type.
- Audit eligible species counts for each boss type and generation before generating teams. Scarce combinations must be identified explicitly rather than silently breaking generation or type restrictions; the exact fallback design for insufficient species is not yet approved.
- This is a design specification, not a claim that the complete teams have been implemented.

## Approved provisional boss-roster fallback (2026-10-15)
- Applies to Gym Leaders and Elite Four; preserve each specialist's established primary type identity.
- Roster selection priority: (1) eligible Pokémon with the primary type, including dual-types; (2) eligible Pokémon of a complementary secondary type; (3) eligible Pokémon fitting a similar thematic identity.
- Generation restriction remains absolute in all three tiers. Prefer unique species and a smaller team over duplicates if the pool is scarce.
- Exact species, secondary-type assignments, and balancing are provisional: flag difficult generation/type combinations for later manual review, rather than blocking the menu and gameplay infrastructure.
- **Implementation status:** design approved only; the current trainer pool substitution does not yet implement primary/secondary/theme ranking or guaranteed strict filtering. Do not label placeholders as finished teams.
