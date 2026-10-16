# FireRed Deluxe UI theme prototype

This branch isolates a proposed visual refresh for Adventure Settings, without merging into main.

## Visual specification
- Light: cream backgrounds, FireRed red header accents, antique-gold window trim, dark brown text.
- Dark: charcoal backgrounds, muted-red accents, warm-gold borders, cream text.
- Generation checklist: five visible rows, nine independent generation toggles, selected counter, help text, Select All, Clear All, Done.
- Intro: short Adventure Settings explanation with a styled title and dialogue frame.
- Options: add UI THEME (LIGHT / DARK), persistent per save, switchable without restarting.
- Theme should apply to the Adventure Settings and Options windows first; avoid altering overworld or battle palettes.

## Status
Design specification only. Not yet implemented or compiled. A playable ROM has **not** been generated. Existing generation/EXP selection and bedroom guard remain unchanged.

## Validation checklist
- Build with the existing GBA workflow.
- Confirm both themes render at native 240x160 without text overlap.
- Test generation checklist, descriptions, scrolling, A/B, Select All/Clear All, and save/reload.
- Test EXP mode and the staircase confirmation gate.
- Confirm UI theme toggles and persists on a real save.

## First playable implementation pass (2026-10-16)
- Generation checklist: new red accent ribbon, selected count, drawn checkbox outlines, retained scrolling and descriptive help.
- Adventure Settings and EXP menus: shared theme-aware menu palette while active, restored on exit to avoid overworld palette changes.
- Options: new UI THEME row with LIGHT/DARK choices, selectable with left/right, persisted in reserved event variable 0x40FE. Theme takes effect for subsequently opened Adventure Settings menus.
- Options header and backdrop updated for the Deluxe prototype.
- No custom bitmap assets yet; this is a palette-and-window based GBA implementation pass.
- CI compilation and emulator playtesting are required before calling this complete.
