# Battle Test Lab — Development Branch

Branch: `feature/battle-testing-lab` (does not modify `main` or `dev/romhack-integration`).

## Prototype implemented
- An Oak's Lab scientist was shifted down one tile; talking to them warps to the room.
- Larger indoor room reuses the Indigo Plateau Pokemon Center 1F layout (25x18); the tiles themselves are not custom.
- Twelve repeatable battle NPCs: Brock, Misty, Lt Surge, Erika, Koga, Sabrina, Blaine, Giovanni, and four Elite Four slots represented by the current Lorelei, Bruno, Agatha, and Lance sprites.
- Exit scientist returns the player to Oak's Lab; a healer restores the party; a supplier offers Rare Candies, Max Revives, and Full Restores; test-team scientist provides six level-55 Pokemon.
- Repeatability uses `cleartrainerflag` before each `trainerbattle_single` encounter to allow replay and avoids the original leader reward event scripts.

## Important limitations
- The twelve encounters currently use **existing canonical FireRed trainer IDs/parties** in `src/data/trainers_frlg.party`. They DO NOT yet use the planned custom teams or Mythical aces Marshadow, Diancie, Volcanion, and Chill Drive Genesect.
- No battle gimmicks are specially enabled for these encounters yet.
- Test-team NPC gives monsters every time; there is no duplicate protection and party/PC capacity still applies. Using it repeatedly may fill storage.
- NPC locations use an existing room layout and **have not been emulator-tested** for wall/furniture collisions; adjust if blocked. Existing layout warp tiles are not used for lab exit: speak to the exit scientist.
- Lab is test-branch-only. Staircase generation-menu bypass remains a temporary test-only workaround.
- No GBA build has been verified for this latest NPC phase.

## Next validation
1. GitHub Actions: make firered, inspect compiler/linker failures.
2. Enter the lab from Oak's Lab; verify player spawn and all 16 NPC placements.
3. Fight each boss twice and verify reset. Check defeat/whiteout behavior.
4. Test healer, items, and level-55 party.
5. Replace canonical trainer parties with the designed 8 Gym and 4 Elite Four configurations and dedicated trainer gimmicks, without affecting release gameplay.
