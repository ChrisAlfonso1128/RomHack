# Local setup quick start

The authoritative platform-specific instructions remain in `INSTALL.md`. This file summarizes the project-specific flow.

## Clone the project

```bash
git clone https://github.com/ChrisAlfonso1128/RomHack.git
cd RomHack
```

## Install build prerequisites

On Windows, follow the WSL/MSYS2 guidance in `INSTALL.md`. On Linux/macOS, follow the matching upstream section.

The standard build uses pret/agbcc. A typical setup keeps `agbcc` beside the RomHack folder:

```bash
cd ..
git clone https://github.com/pret/agbcc
cd agbcc
./build.sh
./install.sh ../RomHack
cd ../RomHack
```

Then build:

```bash
make
```

The generated `pokefirered.gba` is a local build output and must not be committed.

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

Check out the feature branch, run `make`, then launch the resulting ROM in your emulator.

For visual map-layout work, open the repository root in Porymap while on the same feature branch.

## If a build fails

Keep the full terminal output. The first real compiler/linker error is usually more useful than the final `make` failure line.
