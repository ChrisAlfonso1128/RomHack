# Battle Testing Lab (development branch)

Status: Basic explorable test room added, with an Oak's Lab entry NPC and a return guide. It currently reuses the 13x10 FRLG rival-house layout and has no trainer battles, test team stations, or unique map tiles yet. Source changes have not been compiled or emulator-tested.

## Access point
An added scientist coordinator stands in Oak's Lab at map coordinates (12, 8). Talking to this NPC warps to BattleTestLab (5, 6). A scientist guide inside at (6, 4) returns the player to Oak's Lab (11, 8). This is intentionally added only to feature/battle-testing-lab (based on dev/romhack-integration); do not merge to main or release builds.

## Intended room
- Dedicated indoor testing map with a safe return warp to Oak's Lab.
- Eight repeatable Gym Leader stations and four repeatable Elite Four stations.
- Preparation station: balanced test party presets, level presets, healing and PP recovery.
- Mechanic station: Mega Ring + stones, G-Max eligibility, Tera Orb/access, battle rule checks.
- Each lab opponent should call the same canonical trainer party as the actual story encounter, not a duplicate fork.
- Battle reset with no badge/story rewards and no permanent trainer-defeated flags.
- Test fixed Mythicals: Marshadow, Diancie, Volcanion, Chill Drive Genesect (Techno Blast).
- Disable Z-Moves for the designed gameplay; test one-gimmick-per-side rule only after implementation.

## Safe sequence
1. Compile and verify the Oak's Lab scientist and two-way scripted warps in-game.
2. Replace the reused indoor room layout with a dedicated, appropriately sized Battle Lab tilemap when trainer stations are ready.
3. Provide repeatable battle scripts for 12 trainer slots.
4. Supply fixed balanced team presets and mechanic access, with restoration/reset controls.
5. Test gimmick behaviors and generation-restricted party variants.
6. Compile and run the FireRed build, and test in an emulator.

## Current caveats
The source has not been compiled or emulator-tested as part of this initial scaffold. The coordinator is deliberately only a placeholder. Keep work isolated on feature/battle-testing-lab.
