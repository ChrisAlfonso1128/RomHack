# Local setup quick start

The authoritative platform-specific instructions remain in `INSTALL.md`. This file summarizes the RomHack-specific flow.

## Clone the project

```bash
git clone https://github.com/ChrisAlfonso1128/RomHack.git
cd RomHack
```

## Install build prerequisites

On Windows, use the WSL guidance in `INSTALL.md`. RHH Expansion 1.17.0 uses the modern ARM GCC toolchain; the older `agbcc` setup is not required.

## Build the FireRed target

```bash
make firered
```

A successful build produces `pokefirered.gba` in the project root. That file is a local build output and must not be committed.

## Updating local source

Before starting new work:

```bash
git checkout main
git pull
```

For an existing feature branch:

```bash
git checkout <branch-name>
git pull
```

## Testing a pull-request branch

Check out the feature branch, run `make firered`, then launch `pokefirered.gba` in your emulator.

For visual map-layout work, open the repository root in Porymap while on the same feature branch.

## If a build fails

Keep the full terminal output. The first real compiler/linker error is usually more useful than the final `make` failure line.

When reporting a bug, also provide the changelog entry/change trace if the affected feature has one. That gives a direct path to the source files and anchors most likely involved.
