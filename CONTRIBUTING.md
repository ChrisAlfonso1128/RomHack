# Contributing

## Branches

Create a focused branch for each change rather than using the stable branch as a scratch workspace.

Recommended names:

- `feature/v0.5-brock-rework`
- `feature/route-3-trainers`
- `content/rival-dialogue`
- `fix/oak-lab-event`
- `map/viridian-crashsite`
- `chore/upstream-sync`

## Pull requests

Explain what changed, why it changed, what still needs gameplay/visual testing, and whether Porymap or graphics assets were touched. Compilation is necessary but does not replace gameplay testing.

## Changelog

Update `CHANGELOG.md` for player-visible changes.

## ROM and generated-file policy

Do not commit full FireRed/LeafGreen ROM images, local build outputs, save files, emulator states, or ROM dumps.

The upstream repository intentionally tracks a few `data/*.gba` multiboot/source assets. Those are part of the decomp project and should not be confused with a built game ROM.

## Maps

Use Porymap for visual tile/layout work. Review the Git diff before committing so unrelated map/config changes are not included accidentally.

## Upstream sync

Keep upstream updates separate from feature work. Record the imported pret/pokefirered revision in `docs/UPSTREAM.md` whenever the base is intentionally updated.
