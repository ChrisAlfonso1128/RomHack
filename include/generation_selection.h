#ifndef GUARD_GENERATION_SELECTION_H
#define GUARD_GENERATION_SELECTION_H

#include "global.h"

// A zero configuration means All Generations, including for legacy saves.
// Generation values 1-9 use the introduction generation of each species.
// This deliberately uses one persistent event var rather than individual flags.
enum GenerationSelectionMode
{
    GENERATION_MODE_ONLY,
    GENERATION_MODE_UNLOCKED,
};

#define GENERATION_ALL 0

u16 GetGenerationSelectionMask(void);
void SetGenerationSelectionMask(u16 mask);
u8 GetSelectedGeneration(void);
u8 GetSelectedGenerationMode(void);
void SetGenerationSelection(u8 generation, u8 mode);
void StageGenerationSelectionForNewGame(u8 generation, u8 mode);
void ApplyPendingGenerationSelection(void);
u8 GetSpeciesIntroductionGeneration(u16 species);
bool8 IsSpeciesAllowedByGeneration(u16 species);


// Persistent generation selection bits (Gen 1 through Gen 9).
// Read from VAR_GENERATION_SELECTION using GetGenerationSelectionMask().
#define ADVENTURE_GEN_1 (1 << 0)
#define ADVENTURE_GEN_2 (1 << 1)
#define ADVENTURE_GEN_3 (1 << 2)
#define ADVENTURE_GEN_4 (1 << 3)
#define ADVENTURE_GEN_5 (1 << 4)
#define ADVENTURE_GEN_6 (1 << 5)
#define ADVENTURE_GEN_7 (1 << 6)
#define ADVENTURE_GEN_8 (1 << 7)
#define ADVENTURE_GEN_9 (1 << 8)

// Persistent battle mechanics bits in VAR_ADVENTURE_BATTLE_MECHANICS.
// An unset saved marker means the default is all four enabled.
#define ADVENTURE_MECHANIC_MEGA           (1 << 0)
#define ADVENTURE_MECHANIC_DYNAMAX        (1 << 1)
#define ADVENTURE_MECHANIC_GIGANTAMAX     (1 << 2)
#define ADVENTURE_MECHANIC_TERASTALLIZE   (1 << 3)
#define ADVENTURE_MECHANICS_ALL           0x000F
#define ADVENTURE_MECHANICS_SAVED         0x8000

// Read-only accessors for future encounter, trainer, and battle integration.
bool8 IsAdventureGenerationEnabled(u8 generation);
u16 GetAdventureBattleMechanicsMask(void);
bool8 IsAdventureBattleMechanicEnabled(u16 mechanicBit);

#endif // GUARD_GENERATION_SELECTION_H
