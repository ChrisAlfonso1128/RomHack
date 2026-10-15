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

u8 GetSelectedGeneration(void);
u8 GetSelectedGenerationMode(void);
void SetGenerationSelection(u8 generation, u8 mode);
void StageGenerationSelectionForNewGame(u8 generation, u8 mode);
void ApplyPendingGenerationSelection(void);
u8 GetSpeciesIntroductionGeneration(u16 species);
bool8 IsSpeciesAllowedByGeneration(u16 species);

#endif // GUARD_GENERATION_SELECTION_H
