# RomHack

A collaborative Pokémon FireRed ROM-hack project built on the [pret/pokefirered](https://github.com/pret/pokefirered) decompilation.

## Project status

The repository now uses a source-based decomp workflow migrated from the older Hex Maniac Advance (binary-ROM) project. The imported decomp base is recorded in [docs/UPSTREAM.md](docs/UPSTREAM.md).

**Important:** old HMA changes were not automatically converted into source. They are tracked in [docs/LEGACY_MIGRATION.md](docs/LEGACY_MIGRATION.md) and should be ported deliberately, feature by feature.

## Core workflow

1. Start from the latest stable source.
2. Create a focused branch such as `feature/v0.5-brock-rework`.
3. Make source/data/script changes.
4. Update [CHANGELOG.md](CHANGELOG.md) for player-visible changes.
5. Open a pull request.
6. GitHub Actions verifies that the project builds.
7. Mike/Chris perform local gameplay and visual testing in an emulator/Porymap.
8. Revise as needed, then merge.

See [docs/WORKFLOW.md](docs/WORKFLOW.md) for the full remote-first workflow.

## Local tools

- Git / GitHub
- A supported build environment from [INSTALL.md](INSTALL.md)
- [Porymap](https://github.com/huderlem/porymap) for visual map editing
- A GBA emulator such as mGBA for gameplay testing
- Optional graphics tools for sprites, tiles, palettes, and other art assets

For a project-specific quick start, see [docs/LOCAL_SETUP.md](docs/LOCAL_SETUP.md).

## Build

Follow [INSTALL.md](INSTALL.md) to install the required build tools and `agbcc`, then run:

```bash
make
```

A successful local build creates `pokefirered.gba`. Build outputs and commercial ROM images must not be committed. The upstream `data/*.gba` files are intentional decomp source assets and are the exception already handled by the upstream `.gitignore`.

## Collaboration with ChatGPT

Repository-side source work can be handled through GitHub feature branches: trainer data, encounters, text, event scripts, constants, C code, documentation, changelogs, and similar source-controlled changes.

Local interactive work still requires Mike or Chris, especially gameplay testing, visual map-layout editing in Porymap, and pixel/palette work that needs visual judgment.

## Legacy work

The previous HMA changelog is preserved at [docs/legacy-hma-changelog.txt](docs/legacy-hma-changelog.txt). Do not assume a feature listed there exists in the decomp until it is marked as ported in [docs/LEGACY_MIGRATION.md](docs/LEGACY_MIGRATION.md).

## Upstream

This project derives from pret/pokefirered. The imported upstream revision is recorded in [docs/UPSTREAM.md](docs/UPSTREAM.md). Keep upstream-sync work separate from gameplay feature branches.
