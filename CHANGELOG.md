# Changelog

This file tracks source-based decomp development and also serves as a troubleshooting/learning map for Mike and Chris. Legacy Hex Maniac Advance work is preserved separately in `docs/legacy-hma-changelog.txt`.

## Unreleased

### Added
- Migrated the project to RHH pokeemerald-expansion 1.17.0 using the supported FireRed/FRLG build mode.
- Added GitHub collaboration workflow and FireRed build verification.
- Added documentation for local setup, branching, playtesting, legacy migration, and source-location tracing.
- Added a changelog **Change trace** standard so gameplay changes point back to the files and source locations that implement them.

### Changed
- Reworked the Honedge, Trapinch, and Impidimp family level-up learnsets for starter progression, including earlier STAB access, evolution-stage signature moves, and smoother late-game move pacing.
  - Change trace:
    - File: `src/data/pokemon/level_up_learnsets/gen_3.h`
    - Lines: 6725-6779
    - Anchors: `sTrapinchLevelUpLearnset`, `sVibravaLevelUpLearnset`, `sFlygonLevelUpLearnset`
    - Commit: `9aafa05`
    - Notes: Adds early Ground coverage, Dragon Breath on Vibrava evolution, Dragon Claw on Flygon evolution, and later Ground/coverage upgrades.
    - File: `src/data/pokemon/level_up_learnsets/gen_6.h`
    - Lines: 15810-15863
    - Anchors: `sHonedgeLevelUpLearnset`, `sDoubladeLevelUpLearnset`, `sAegislashLevelUpLearnset`
    - Commit: `5d1a9f4`
    - Notes: Delays setup power, gives earlier Ghost/Steel STAB, Shadow Claw on Doublade evolution, and King's Shield on Aegislash evolution.
    - File: `src/data/pokemon/level_up_learnsets/gen_8.h`
    - Lines: 19595-19652
    - Anchors: `sImpidimpLevelUpLearnset`, `sMorgremLevelUpLearnset`, `sGrimmsnarlLevelUpLearnset`
    - Commit: `0bb41ba`
    - Notes: Adds early Fairy STAB, False Surrender on Morgrem evolution, Spirit Break on Grimmsnarl evolution, and preserves utility-focused progression.

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
