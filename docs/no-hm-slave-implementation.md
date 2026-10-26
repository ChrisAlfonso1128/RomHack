# No HM Slave / Modern Field Moves — implementation checklist

Issue: #17
Branch: `feature/no-hm-slave-testing` (based on `dev/romhack-integration`)

## Approved behavior
- New games default to Modern; legacy saves with no initialized preference remain Classic.
- Modern field use requires the existing badge plus any explicitly justified story/map gate; HM possession, learned moves and HM-compatible species are not required.
- Classic preserves original behavior.
- Hybrid activation: A on Cut/Surf/Strength/Rock Smash obstacles; automatic Waterfall; dedicated Fly/Flash menu.
- Preserve confirmation prompts, map restrictions, effects, puzzles, follower safety and save/load state.
- No merging to development or main without review.

## Verified integration points on this branch
- `src/field_move.c` and `include/field_move.h`: existing badge-based unlock mapping.
- `src/scrcmd.c`: `checkfieldmove` script command returns a party index or `PARTY_SIZE` sentinel. Modern must never feed the sentinel to nickname/field-effect routines.
- `src/field_control_avatar.c` and `src/field_player_avatar.c`: water, waterfall and boulder traversal.
- `src/start_menu.c`: pause menu; currently has no Field Moves action.
- `src/option_menu.c`: existing options interface; currently no Field Moves setting.
- `src/new_game.c`: new-game initialization.
- `src/generation_selection.c`: existing persistent var-based generation selection; do not reuse its var or bits.

## Implementation sequence
1. Audit save layout and reserve an explicitly verified persistent setting + initialized marker. Implement safe legacy fallback.
2. Wire new-game Modern default and a Classic/Modern option in the appropriate Adventure Settings UI.
3. Implement a single mode-aware eligibility helper (badge, story flag and context) and route scripts and overworld checks through it.
4. Add safe neutral animations/strings for field effects without a selected Pokémon; audit followers and party index accesses.
5. Add Fly/Flash Field Moves pause menu in Modern mode, including destination restrictions and locked feedback.
6. Audit and document map-specific progression gates; do not silently require HM acquisition.
7. Run compilation, save migration tests, all seven field-move scenarios, mode switching, and softlock regression; update PATCH_NOTES.md.

## Safety conditions
- Do not use unverified save bits or assume an uninitialized value represents Modern.
- Do not pass `PARTY_SIZE` as a real Pokémon index.
- Do not modify `main` or `dev/romhack-integration`.
- Do not mark Issue #17 complete before a working ROM and regression checks.
