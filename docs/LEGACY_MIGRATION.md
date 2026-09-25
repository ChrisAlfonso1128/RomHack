# Legacy HMA migration checklist

The previous project was edited directly as a ROM with Hex Maniac Advance. Those binary changes are **not automatically present** in the new decomp source.

The original changelog is preserved in `docs/legacy-hma-changelog.txt`.

## Features to review and port

- [x] Starter trio: Honedge / Trapinch / Impidimp.
- [x] Starter-selection dialogue changes.
- [x] Honedge -> Doublade evolution ported and intentionally redesigned to level 18.
- [x] Doublade -> Aegislash evolution ported and intentionally redesigned to level 36 instead of a Dusk Stone.
- [ ] Oak's Lab NPC that gives one Exp. Share.
- [x] Oak's Lab opening rival trainer/team changes.
- [x] All later rival teams updated for the custom starter choices.
- [ ] Chris's & Sam's House map.
- [ ] Sam NPC in the house.
- [ ] Chris NPC in the house.
- [ ] Pallet Town custom-house touchups.
- [ ] Oak intro changes (legacy work was WIP).
- [ ] Viridian City meteor crash site (legacy work was WIP).
- [ ] Custom spawn-location change.
- [ ] "God Bidoof" test/custom content.
- [ ] Bidoofite.
- [ ] Level-100 Bidoof test NPC.
- [ ] Bidoofite and Mega Ring test item balls.
- [ ] Missing God Bidoof sprites/text/stats identified in the legacy changelog.

## Migration rule

Mark an item complete only after the equivalent source change has been committed, the project builds successfully, and the feature has been verified in-game.

Treat the old binary build as a behavior/visual reference, not as the new source of truth.
