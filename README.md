# RomHack

A collaborative Pokémon FireRed ROM-hack project built on **RHH pokeemerald-expansion 1.17.0** using its supported **FireRed/FRLG build mode**.

## Project status

The repository has been migrated from the older Hex Maniac Advance binary-ROM workflow to a source-based decomp workflow. The exact upstream release/commit is pinned in [docs/UPSTREAM.md](docs/UPSTREAM.md).

The old HMA changes were **not** automatically converted into source. They are tracked in [docs/LEGACY_MIGRATION.md](docs/LEGACY_MIGRATION.md) and should be ported deliberately, feature by feature.

## Why this base

RHH Expansion gives the project FireRed/Kanto support while also providing the modern Pokémon/mechanics infrastructure needed by the existing design, including later-generation species, Fairy typing, modern moves/abilities, and Mega Evolution support.

## Core workflow

1. Start from the latest stable source on `main`.
2. Create a focused branch such as `feature/v0.5-brock-rework`.
3. Make source/data/script changes.
4. Update [CHANGELOG.md](CHANGELOG.md) for player-visible or troubleshooting-relevant changes.
5. Include a change trace with file path, line range, and a stable code/script anchor.
6. Open a pull request.
7. GitHub Actions verifies that the FireRed target builds.
8. Mike/Chris perform local gameplay and visual testing in an emulator/Porymap.
9. Revise as needed, then merge.

See [docs/WORKFLOW.md](docs/WORKFLOW.md) and [docs/CHANGELOG_GUIDE.md](docs/CHANGELOG_GUIDE.md).

## Local tools

- Git / GitHub
- A supported ARM/GBA build environment from [INSTALL.md](INSTALL.md)
- [Porymap](https://github.com/huderlem/porymap) for visual map editing
- A GBA emulator such as mGBA for gameplay testing
- Optional graphics tools for sprites, tiles, palettes, and other art assets

For a project-specific quick start, see [docs/LOCAL_SETUP.md](docs/LOCAL_SETUP.md).

## Build

After installing the prerequisites described in [INSTALL.md](INSTALL.md), build the FireRed target with:

```bash
make firered
```

A successful local build creates the FireRed ROM output for the project. Build outputs, commercial ROM images, save files, and emulator states must not be committed.

## Collaboration with ChatGPT

Repository-side source work can be handled through GitHub feature branches: trainer data, encounters, Pokémon data, text, event scripts, constants, C code, documentation, changelogs, and similar source-controlled changes.

Local interactive work still requires Mike or Chris, especially gameplay testing, visual map-layout editing in Porymap, and pixel/palette work that needs visual judgment.

## Legacy work

The previous HMA changelog is preserved at [docs/legacy-hma-changelog.txt](docs/legacy-hma-changelog.txt). Do not assume a legacy feature exists in the decomp until it is marked as ported in [docs/LEGACY_MIGRATION.md](docs/LEGACY_MIGRATION.md).

## Upstream

This project derives from RHH pokeemerald-expansion. The exact pinned release and commit are recorded in [docs/UPSTREAM.md](docs/UPSTREAM.md). Keep upstream-sync work separate from gameplay feature branches.
