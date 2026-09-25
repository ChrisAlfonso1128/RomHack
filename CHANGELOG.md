# Changelog

This file tracks source-based decomp development and also serves as a troubleshooting/learning map for Mike and Chris. Legacy Hex Maniac Advance work is preserved separately in `docs/legacy-hma-changelog.txt`.

## Unreleased

### Added
- Migrated the project to RHH pokeemerald-expansion 1.17.0 using the supported FireRed/FRLG build mode.
- Added GitHub collaboration workflow and FireRed build verification.
- Added documentation for local setup, branching, playtesting, legacy migration, and source-location tracing.
- Added a changelog **Change trace** standard so gameplay changes point back to the files and source locations that implement them.
- Documented the built-in development/debug menu and its starter-testing utilities in `docs/DEV_TOOLS.md`.

### Changed
- Reworked the Honedge, Trapinch, and Impidimp family level-up learnsets for starter progression, including earlier STAB access, evolution-stage signature moves, and smoother late-game move pacing.
  - Change trace:
    - File: `src/data/pokemon/level_up_learnsets/gen_3.h`
    - Anchors: `sTrapinchLevelUpLearnset`, `sVibravaLevelUpLearnset`, `sFlygonLevelUpLearnset`
    - Commit: `f6e8327`
    - Notes: Adds earlier Ground coverage, Dragon Breath on Vibrava evolution, Dragon Claw on Flygon evolution, and smoother later Ground/coverage progression.
    - File: `src/data/pokemon/level_up_learnsets/gen_6.h`
    - Anchors: `sHonedgeLevelUpLearnset`, `sDoubladeLevelUpLearnset`, `sAegislashLevelUpLearnset`
    - Commit: `fb788dd`
    - Notes: Gives earlier Ghost/Steel STAB, Shadow Claw on Doublade evolution, and King's Shield on Aegislash evolution while avoiding excessive early setup power.
    - File: `src/data/pokemon/level_up_learnsets/gen_8.h`
    - Anchors: `sImpidimpLevelUpLearnset`, `sMorgremLevelUpLearnset`, `sGrimmsnarlLevelUpLearnset`
    - Commit: `f8c14d5`
    - Notes: Adds earlier Fairy STAB, False Surrender on Morgrem evolution, Spirit Break on Grimmsnarl evolution, and keeps the line utility-focused.

- Rebalanced the three planned starter lines so their early- and mid-game progression is comparable while preserving their distinct roles.
  - Honedge line:
    - Honedge is now 310 BST (45/75/90/35/40/25), uses Medium Slow growth, and evolves at level 18.
    - Doublade is now 410 BST (59/95/120/40/61/35), uses Medium Slow growth, and evolves into Aegislash at level 36 instead of requiring a Dusk Stone.
    - Aegislash keeps its existing battle stats and now uses Medium Slow growth.
  - Trapinch line:
    - Trapinch is now 310 BST (45/90/50/35/55/35), uses Medium Slow growth, and evolves at level 18.
    - Vibrava is now 410 BST (55/80/65/70/70/70), uses Medium Slow growth, and evolves into Flygon at level 36.
    - Flygon keeps its existing battle stats and Medium Slow growth.
  - Impidimp line:
    - Impidimp is now 310 BST (45/60/45/60/50/50), uses Medium Slow growth, and evolves at level 18.
    - Morgrem is now 410 BST (65/75/55/80/60/75), uses Medium Slow growth, and evolves into Grimmsnarl at level 36.
    - Grimmsnarl keeps its existing battle stats and now uses Medium Slow growth.
  - Change trace:
    - File: `src/data/pokemon/species_info/gen_3_families.h`
    - Lines: 7330-7552
    - Anchors: `SPECIES_TRAPINCH`, `SPECIES_VIBRAVA`, `SPECIES_FLYGON`
    - Commit: `0609c8d`
    - File: `src/data/pokemon/species_info/gen_6_families.h`
    - Lines: 3045-3328
    - Anchors: `SPECIES_HONEDGE`, `SPECIES_DOUBLADE`, `SPECIES_AEGISLASH_SHIELD`, `SPECIES_AEGISLASH_BLADE`
    - Commit: `2538515`
    - File: `src/data/pokemon/species_info/gen_8_families.h`
    - Lines: 4536-4729
    - Anchors: `SPECIES_IMPIDIMP`, `SPECIES_MORGREM`, `SPECIES_GRIMMSNARL`
    - Commit: `1283c2f`
    - Notes: Final-stage base stats were intentionally left unchanged; the balance pass targets starter progression rather than global Pokédex rebalance.

- Ported the custom starter trio into FireRed's Oak's Lab starter scene: Honedge, Trapinch, and Impidimp now replace the vanilla starter choices, with matching confirmation text and the first rival battle updated to use the corresponding custom starter.
  - Starter/rival cycle: Honedge -> rival Impidimp; Trapinch -> rival Honedge; Impidimp -> rival Trapinch.
  - Change trace:
    - File: `data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc`
    - Lines: 1071-1073, 1214-1216, 1227-1229, 1404-1419
    - Anchors: `PalletTown_ProfessorOaksLab_EventScript_BulbasaurPokeBall`, `PalletTown_ProfessorOaksLab_EventScript_SquirtlePokeBall`, `PalletTown_ProfessorOaksLab_EventScript_CharmanderPokeBall`, `PalletTown_ProfessorOaksLab_Text_OakChoosingCharmander`
    - Commit: `8bfc9a1`
    - File: `src/data/trainers_frlg.party`
    - Lines: 4431-4473
    - Anchors: `TRAINER_RIVAL_OAKS_LAB_SQUIRTLE`, `TRAINER_RIVAL_OAKS_LAB_BULBASAUR`, `TRAINER_RIVAL_OAKS_LAB_CHARMANDER`
    - Commit: `67bad6b`
    - Notes: Internal trainer/script labels retain their vanilla names for compatibility; the actual species used in-game are the custom trio.

### Tested
- Starter-selection Poké Balls correctly award Honedge, Trapinch, and Impidimp in Oak's Lab.
- The opening rival battle selects the intended opposing starter.
- All three starter lines were verified in-game to evolve at levels 18 and 36.
- Starter learnset progression was spot-checked through evolution, including Honedge-line moves such as Aerial Ace, Metal Sound, Slash, Night Slash, and Retaliate.
- The game builds and runs normally with the starter package applied.

### Migration
- Pinned RHH Expansion release `expansion/1.17.0` at commit `e8bd1cd7b03fc032ea37e3ecd38b379b5d01a1e7`.
- Verified the FireRed target builds successfully in GitHub Actions.
- Preserved the earlier vanilla pokefirered setup on an archive branch.
- Legacy HMA gameplay features are not yet considered ported. See `docs/LEGACY_MIGRATION.md`.

## Change-trace format

Gameplay-facing and troubleshooting-relevant entries should use this structure:

```md
- Description of the change.
  - Change trace:
    - File: `path/to/file`
    - Lines: 100-125
    - Anchor: `SEARCHABLE_SYMBOL_OR_SCRIPT_LABEL`
    - Commit: `abc1234`
    - Notes: Optional troubleshooting context.
```

For multi-file features, list each materially involved file. Line ranges describe the implementing commit and may drift later, so the **Anchor** is the durable lookup reference.

See `docs/CHANGELOG_GUIDE.md` for the full standard.

## Versioning convention

Milestone builds may use tags such as `v0.5`, `v0.6`, and so on. Day-to-day work should remain under **Unreleased** until a milestone is intentionally cut.
