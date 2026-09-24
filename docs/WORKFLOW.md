# Development workflow

## Remote-first workflow

This project is designed so planning and most source changes can happen without Mike being at the development PC.

1. Mike describes a change from ChatGPT/mobile.
2. A dedicated GitHub feature branch is created from the current stable source.
3. Source files are inspected and modified.
4. Relevant documentation and `CHANGELOG.md` are updated.
5. A pull request is opened.
6. GitHub Actions performs a clean build check.
7. Mike or Chris checks out the branch locally and performs gameplay/visual testing.
8. Feedback is applied to the same branch.
9. Once approved and build-tested, the pull request is merged.

## Work that can usually be handled directly in the repository

- trainers, parties, levels, items, moves, and AI configuration;
- wild encounters;
- Pokémon data, evolution data, learnsets, and constants;
- dialogue and text;
- event scripts, flags, variables, and conditions;
- NPC/event metadata;
- many map connections and metadata changes;
- items and item behavior;
- C changes for mechanics;
- bug fixes and compiler-error diagnosis;
- documentation, changelogs, issues, branches, commits, and pull requests.

## Work that remains primarily local/interactive

- playing the ROM and judging game feel;
- validating event sequences in-game;
- visual map-layout editing in Porymap;
- pixel-art and palette editing where visual judgment is required;
- checking sprite alignment/animations;
- emulator-specific behavior and save-file testing.

## Review standard

A feature is not complete merely because it compiles. Gameplay-facing changes should build successfully and receive appropriate in-game/visual testing.

## Merge discipline

Prefer small, focused pull requests. If Chris and Mike are working simultaneously, use separate branches and resolve conflicts before gameplay approval.
