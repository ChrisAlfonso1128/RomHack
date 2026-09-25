# Contributing

## Branches

Create a focused branch for each change rather than using `main` as a scratch workspace.

Recommended names:

- `feature/v0.5-brock-rework`
- `feature/route-3-trainers`
- `content/rival-dialogue`
- `fix/oak-lab-event`
- `map/viridian-crashsite`
- `chore/upstream-sync`

## Pull requests

Explain what changed, why it changed, what still needs gameplay/visual testing, and whether Porymap or graphics assets were touched. Compilation is necessary but does not replace gameplay testing.

## Changelog and change traces

Update `CHANGELOG.md` for player-visible or troubleshooting-relevant changes.

Every meaningful changelog entry should include:

- repository-relative file path;
- exact line range from the implementing commit;
- a stable searchable anchor such as a function, script label, constant, trainer ID, or table entry;
- commit/PR reference when available;
- troubleshooting notes when useful.

See `docs/CHANGELOG_GUIDE.md` for the standard format.

## ROM and generated-file policy

Do not commit full FireRed/LeafGreen ROM images, local build outputs, save files, emulator states, or ROM dumps.

## Maps

Use Porymap for visual tile/layout work. Review the Git diff before committing so unrelated map/config changes are not included accidentally.

## Upstream sync

Keep upstream updates separate from feature work. Record the pinned RHH Expansion release and commit in `docs/UPSTREAM.md` whenever the base is intentionally updated.
