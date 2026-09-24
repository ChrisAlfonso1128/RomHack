# Legacy HMA migration checklist

The previous project was edited directly as a ROM with Hex Maniac Advance. Those binary changes are **not automatically present** in the new decomp source.

The original changelog is preserved in `docs/legacy-hma-changelog.txt`.

## Features to review and port

- [ ] Starter trio: Honedge / Trapinch / Impidimp.
- [ ] Starter-selection dialogue changes.
- [ ] Honedge -> Doublade at level 25.
- [ ] Doublade -> Aegislash at level 45.
- [ ] Oak's Lab NPC that gives one Exp. Share.
- [ ] Oak's Lab rival trainer-ID/team changes.
- [ ] All rival teams updated for the custom starter choices.
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
