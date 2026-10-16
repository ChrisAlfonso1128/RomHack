# Patch Notes

## Unreleased — QoL move-effectiveness indicators
Branch: `qol-move-effectiveness`

### Battle interface
- Updated visibility setting to **always** show effectiveness for all opponent species, including those not yet seen or caught; neutral moves remain blank and status moves have no indicator.
- Switched yellow/orange indicator strings to named charmap palette indices to avoid ambiguous numeric color tokens.
- Replaced the original effectiveness icons with compact, color-coded symbols in the move-selection PP area.
- Final mapping: **4×** = light-blue `+`; **2×** = green `+`; **1×** = blank; **½×** = yellow `-`; **¼×** = orange `-`; **0×** = red `X`.
- Kept existing type-effectiveness calculations, status-move handling, and visibility rules for opponents not yet seen.
- Assigned the PP indicator window a dedicated background palette, allowing yellow and orange text without recoloring neighboring move-menu windows.

### Build and validation
- Fixed GitHub Actions build #40 failure: corrected the RGB header include to `constants/rgb.h` (the project does not have a top-level `rgb.h`). Rebuild verification pending.
- Fixed GitHub Actions build #38 failure: added the missing `rgb.h` include for the `RGB()` macro used by the new yellow/orange palette. Rebuild verification pending.
- An earlier indicator revision successfully compiled in GitHub Actions (build #31) and was confirmed working in gameplay by the project team.
- **This latest six-color revision has not yet been verified in an emulator.** Confirm compilation and visual behavior before merging.
- GitHub Actions workflow uploads a playable `pokefirered.gba` artifact named `firered-qol-playtest`.

### History
- Initial implementation: green `+`, dark `+`, dark `-`, red `X`.
- Follow-up: distinguished 4× from 2×, and made 1× blank.
- Current revision: six separate multiplier tiers and the requested colors.

Future changes should be documented here alongside the source changes.
