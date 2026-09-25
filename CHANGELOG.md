# Changelog

This file tracks source-based decomp development and also serves as a troubleshooting/learning map for Mike and Chris. Legacy Hex Maniac Advance work is preserved separately in `docs/legacy-hma-changelog.txt`.

## Unreleased

### Added
- Migrated the project to RHH pokeemerald-expansion 1.17.0 using the supported FireRed/FRLG build mode.
- Added GitHub collaboration workflow and FireRed build verification.
- Added documentation for local setup, branching, playtesting, legacy migration, and source-location tracing.
- Added a changelog **Change trace** standard so gameplay changes point back to the files and source locations that implement them.

### Changed
- Rebalanced the Honedge starter line for a smoother starter-style progression: Honedge and Doublade were normalized to 310/410 BST, the full line now uses Medium Slow growth, and evolutions occur at levels 18 and 36 instead of 35 plus a Dusk Stone.
  - Change trace:
    - File: `src/data/pokemon/species_info/gen_6_families.h`
    - Lines: 3045-3328
    - Anchors: `SPECIES_HONEDGE`, `SPECIES_DOUBLADE`, `SPECIES_AEGISLASH_SHIELD`, `SPECIES_AEGISLASH_BLADE`
    - Commit: `7e4c11f`
    - Notes: Aegislash final-form battle stats remain unchanged; only its growth curve was normalized.
- Rebalanced the Trapinch starter line: Trapinch and Vibrava were normalized to 310/410 BST and now evolve at levels 18 and 36. Flygon's final stats and the line's existing Medium Slow growth rate remain unchanged.
  - Change trace:
    - File: `src/data/pokemon/species_info/gen_3_families.h`
    - Lines: 7330-7552
    - Anchors: `SPECIES_TRAPINCH`, `SPECIES_VIBRAVA`, `SPECIES_FLYGON`
    - Commit: `a3965d5`
- Rebalanced the Impidimp starter line: Impidimp and Morgrem were normalized to 310/410 BST, the full line now uses Medium Slow growth, and evolutions occur at levels 18 and 36.
  - Change trace:
    - File: `src/data/pokemon/species_info/gen_8_families.h`
    - Lines: 4536-4727
    - Anchors: `SPECIES_IMPIDIMP`, `SPECIES_MORGREM`, `SPECIES_GRIMMSNARL`
    - Commit: `b14ce4b`
    - Notes: Grimmsnarl's final stats remain unchanged.

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
