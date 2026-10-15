#include "global.h"
#include "generation_selection.h"
#include "event_data.h"
#include "pokemon.h"
#include "constants/vars.h"

// Bit layout: bits 0-3 generation (0 = all, 1-9 = selected);
// bit 4 = unlocked mode. Reserved bits are ignored.
#define GENERATION_VALUE_MASK 0x000F
#define GENERATION_MODE_BIT   0x0010

static EWRAM_DATA u8 sPendingGeneration = GENERATION_ALL;
static EWRAM_DATA u8 sPendingMode = GENERATION_MODE_ONLY;

u8 GetSelectedGeneration(void)
{
    u16 value = VarGet(VAR_GENERATION_SELECTION);
    u8 generation = value & GENERATION_VALUE_MASK;

    // Unknown or unset saves safely use the full species pool.
    return generation <= 9 ? generation : GENERATION_ALL;
}

u8 GetSelectedGenerationMode(void)
{
    return (VarGet(VAR_GENERATION_SELECTION) & GENERATION_MODE_BIT)
        ? GENERATION_MODE_UNLOCKED : GENERATION_MODE_ONLY;
}

void SetGenerationSelection(u8 generation, u8 mode)
{
    u16 value = 0;

    if (generation >= 1 && generation <= 9)
    {
        value = generation;
        if (mode == GENERATION_MODE_UNLOCKED)
            value |= GENERATION_MODE_BIT;
    }

    VarSet(VAR_GENERATION_SELECTION, value);
}

void StageGenerationSelectionForNewGame(u8 generation, u8 mode)
{
    sPendingGeneration = generation;
    sPendingMode = mode;
}

void ApplyPendingGenerationSelection(void)
{
    SetGenerationSelection(sPendingGeneration, sPendingMode);
    sPendingGeneration = GENERATION_ALL;
    sPendingMode = GENERATION_MODE_ONLY;
}

u8 GetSpeciesIntroductionGeneration(u16 species)
{
    u16 dexNum;

    if (species == SPECIES_NONE || species >= NUM_SPECIES)
        return 0;

    // National Dex numbers preserve the introduction generation for
    // ordinary species, later-generation evolutions and regional forms.
    dexNum = SpeciesToNationalPokedexNum(species);
    if (dexNum == 0) return 0;
    if (dexNum <= 151) return 1;
    if (dexNum <= 251) return 2;
    if (dexNum <= 386) return 3;
    if (dexNum <= 493) return 4;
    if (dexNum <= 649) return 5;
    if (dexNum <= 721) return 6;
    if (dexNum <= 809) return 7;
    if (dexNum <= 905) return 8;
    if (dexNum <= 1025) return 9;
    return 0;
}

bool8 IsSpeciesAllowedByGeneration(u16 species)
{
    u8 generation = GetSelectedGeneration();
    u8 speciesGeneration = GetSpeciesIntroductionGeneration(species);

    if (speciesGeneration == 0)
        return FALSE;
    if (generation == GENERATION_ALL)
        return TRUE;
    if (GetSelectedGenerationMode() == GENERATION_MODE_UNLOCKED)
        return speciesGeneration <= generation;
    return speciesGeneration == generation;
}
