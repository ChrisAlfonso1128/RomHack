#include "global.h"
#include "main.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_specials.h"
#include "item.h"
#include "menu.h"
#include "palette.h"
#include "script.h"
#include "script_menu.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "list_menu.h"
#include "malloc.h"
#include "util.h"
#include "item_icon.h"
#include "pokemon_icon.h"
#include "generation_selection.h"
#include "constants/vars.h"
#include "constants/rgb.h"
#include "constants/field_specials.h"
#include "constants/items.h"
#include "constants/script_menu.h"
#include "constants/seagallop.h"
#include "constants/songs.h"

#include "data/script_menu.h"

struct DynamicListMenuEventArgs
{
    struct ListMenuTemplate *list;
    u16 selectedItem;
    u8 windowId;
};

typedef void (*DynamicListCallback)(struct DynamicListMenuEventArgs *eventArgs);

struct DynamicListMenuEventCollection
{
    DynamicListCallback OnInit;
    DynamicListCallback OnSelectionChanged;
    DynamicListCallback OnDestroy;
};

static EWRAM_DATA u8 sProcessInputDelay = 0;
static EWRAM_DATA u8 sDynamicMenuEventId = 0;
static EWRAM_DATA struct DynamicMultichoiceStack *sDynamicMultiChoiceStack = NULL;
static EWRAM_DATA u16 *sDynamicMenuEventScratchPad = NULL;

static u8 sLilycoveSSTidalSelections[SSTIDAL_SELECTION_COUNT];

static void FreeListMenuItems(struct ListMenuItem *items, u32 count);
static void Task_HandleScrollingMultichoiceInput(u8 taskId);
static void Task_HandleMultichoiceInput(u8 taskId);
static void Task_GenerationChecklist(u8 taskId);
static void OpenGenerationChecklist(void);
static void Task_DeluxeExpMenu(u8 taskId);
static void OpenDeluxeExpMenu(u8 defaultChoice);
static void Task_HandleYesNoInput(u8 taskId);
static void Task_HandleMultichoiceGridInput(u8 taskId);
static void DrawMultichoiceMenuDynamic(u8 left, u8 top, u8 argc, struct ListMenuItem *items, bool8 ignoreBPress, u32 initialRow, u8 maxBeforeScroll, u32 callbackSet);
static void DrawMultichoiceMenu(u8 left, u8 top, u8 multichoiceId, bool8 ignoreBPress, u8 cursorPos);
static void InitMultichoiceCheckWrap(bool8 ignoreBPress, u8 count, u8 windowId, u8 multichoiceId);
static void DrawLinkServicesMultichoiceMenu(u8 multichoiceId);
static void CreatePCMultichoice(void);
static void CreateLilycoveSSTidalMultichoice(void);
static bool8 IsPicboxClosed(void);
static void CreateStartMenuForPokenavTutorial(void);
static void InitMultichoiceNoWrap(bool8 ignoreBPress, u8 unusedCount, u8 windowId, u8 multichoiceId);
static void MultichoiceDynamicEventDebug_OnInit(struct DynamicListMenuEventArgs *eventArgs);
static void MultichoiceDynamicEventDebug_OnSelectionChanged(struct DynamicListMenuEventArgs *eventArgs);
static void MultichoiceDynamicEventDebug_OnDestroy(struct DynamicListMenuEventArgs *eventArgs);
static void MultichoiceDynamicEventShowSprite_OnInit(struct DynamicListMenuEventArgs *eventArgs);
static void MultichoiceDynamicEventShowItem_OnSelectionChanged(struct DynamicListMenuEventArgs *eventArgs);
static void MultichoiceDynamicEventShowPkmn_OnSelectionChanged(struct DynamicListMenuEventArgs *eventArgs);
static void MultichoiceDynamicEventShowSprite_OnDestroy(struct DynamicListMenuEventArgs *eventArgs);

static const struct DynamicListMenuEventCollection sDynamicListMenuEventCollections[] =
{
    [DYN_MULTICHOICE_CB_DEBUG] =
    {
        .OnInit = MultichoiceDynamicEventDebug_OnInit,
        .OnSelectionChanged = MultichoiceDynamicEventDebug_OnSelectionChanged,
        .OnDestroy = MultichoiceDynamicEventDebug_OnDestroy
    },
    [DYN_MULTICHOICE_CB_SHOW_ITEM] =
    {
        .OnInit = MultichoiceDynamicEventShowSprite_OnInit,
        .OnSelectionChanged = MultichoiceDynamicEventShowItem_OnSelectionChanged,
        .OnDestroy = MultichoiceDynamicEventShowSprite_OnDestroy
    },
    [DYN_MULTICHOICE_CB_SHOW_PKMN] =
    {
        .OnInit = MultichoiceDynamicEventShowSprite_OnInit,
        .OnSelectionChanged = MultichoiceDynamicEventShowPkmn_OnSelectionChanged,
        .OnDestroy = MultichoiceDynamicEventShowSprite_OnDestroy
    }
};

static const struct ListMenuTemplate sScriptableListMenuTemplate =
{
    .item_X = 8,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
};

bool8 ScriptMenu_MultichoiceDynamic(u8 left, u8 top, u8 argc, struct ListMenuItem *items, bool8 ignoreBPress, u8 maxBeforeScroll, u32 initialRow, u32 callbackSet)
{
    if (FuncIsActiveTask(Task_HandleMultichoiceInput) == TRUE)
    {
        FreeListMenuItems(items, argc);
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        DrawMultichoiceMenuDynamic(left, top, argc, items, ignoreBPress, initialRow, maxBeforeScroll, callbackSet);
        return TRUE;
    }
}

bool8 ScriptMenu_Multichoice(u8 left, u8 top, u8 multichoiceId, bool8 ignoreBPress)
{
    if (multichoiceId == MULTI_ADVENTURE_GEN_FIRST)
    {
        if (FuncIsActiveTask(Task_GenerationChecklist))
            return FALSE;
        gSpecialVar_Result = 0xFF;
        OpenGenerationChecklist();
        return TRUE;
    }
    if (multichoiceId == MULTI_ADVENTURE_EXP_SHARE)
    {
        if (FuncIsActiveTask(Task_DeluxeExpMenu))
            return FALSE;
        gSpecialVar_Result = 0xFF;
        OpenDeluxeExpMenu(0);
        return TRUE;
    }
    if (FuncIsActiveTask(Task_HandleMultichoiceInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        DrawMultichoiceMenu(left, top, multichoiceId, ignoreBPress, 0);
        return TRUE;
    }
}

bool8 ScriptMenu_MultichoiceWithDefault(u8 left, u8 top, u8 multichoiceId, bool8 ignoreBPress, u8 defaultChoice)
{
    if (multichoiceId == MULTI_ADVENTURE_EXP_SHARE)
    {
        if (FuncIsActiveTask(Task_DeluxeExpMenu))
            return FALSE;
        gSpecialVar_Result = 0xFF;
        OpenDeluxeExpMenu(defaultChoice);
        return TRUE;
    }
    if (FuncIsActiveTask(Task_HandleMultichoiceInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        DrawMultichoiceMenu(left, top, multichoiceId, ignoreBPress, defaultChoice);
        return TRUE;
    }
}

static void MultichoiceDynamicEventDebug_OnInit(struct DynamicListMenuEventArgs *eventArgs)
{
    DebugPrintf("OnInit: %d", eventArgs->windowId);
}

static void MultichoiceDynamicEventDebug_OnSelectionChanged(struct DynamicListMenuEventArgs *eventArgs)
{
    DebugPrintf("OnSelectionChanged: %d", eventArgs->selectedItem);
}

static void MultichoiceDynamicEventDebug_OnDestroy(struct DynamicListMenuEventArgs *eventArgs)
{
    DebugPrintf("OnDestroy: %d", eventArgs->windowId);
}

#define sAuxWindowId sDynamicMenuEventScratchPad[0]
#define sSpriteId sDynamicMenuEventScratchPad[1]
#define TAG_CB_SPRITE_ICON 3000

static void MultichoiceDynamicEventShowSprite_OnInit(struct DynamicListMenuEventArgs *eventArgs)
{
    struct WindowTemplate *template = &gWindows[eventArgs->windowId].window;
    u32 baseBlock = template->baseBlock + template->width * template->height;
    struct WindowTemplate auxTemplate = CreateWindowTemplate(0, template->tilemapLeft + template->width + 2, template->tilemapTop, 4, 4, 15, baseBlock);
    u32 auxWindowId = AddWindow(&auxTemplate);
    SetStandardWindowBorderStyle(auxWindowId, FALSE);
    FillWindowPixelBuffer(auxWindowId, 0x11);
    CopyWindowToVram(auxWindowId, COPYWIN_FULL);
    sAuxWindowId = auxWindowId;
    sSpriteId = MAX_SPRITES;
}

static void FreeSpriteIfUsed(void)
{
    if (sSpriteId != MAX_SPRITES)
    {
        FreeSpriteTilesByTag(TAG_CB_SPRITE_ICON);
        FreeSpritePaletteByTag(TAG_CB_SPRITE_ICON);
        DestroySprite(&gSprites[sSpriteId]);
    }
}

static void ChangeSpriteOnSelection(struct DynamicListMenuEventArgs *eventArgs, u32 x, u32 y)
{
    struct WindowTemplate *template = &gWindows[eventArgs->windowId].window;
    x += template->tilemapLeft * 8 + template->width * 8;
    y += template->tilemapTop * 8;

    gSprites[sSpriteId].oam.priority = 0;
    gSprites[sSpriteId].x = x;
    gSprites[sSpriteId].y = y;
}

static void MultichoiceDynamicEventShowItem_OnSelectionChanged(struct DynamicListMenuEventArgs *eventArgs)
{
    FreeSpriteIfUsed();
    sSpriteId = AddItemIconSprite(TAG_CB_SPRITE_ICON, TAG_CB_SPRITE_ICON, eventArgs->selectedItem);
    if (sSpriteId != MAX_SPRITES)
    {
        ChangeSpriteOnSelection(eventArgs, 36, 20);
    }
}

static void MultichoiceDynamicEventShowPkmn_OnSelectionChanged(struct DynamicListMenuEventArgs *eventArgs)
{
    FreeSpriteIfUsed();
    sSpriteId = CreateTaggedMonIcon(TAG_CB_SPRITE_ICON, TAG_CB_SPRITE_ICON, eventArgs->selectedItem);
    if (sSpriteId != MAX_SPRITES)
    {
        ChangeSpriteOnSelection(eventArgs, 32, 14);
    }
}

static void MultichoiceDynamicEventShowSprite_OnDestroy(struct DynamicListMenuEventArgs *eventArgs)
{
    ClearStdWindowAndFrame(sAuxWindowId, TRUE);
    RemoveWindow(sAuxWindowId);

    if (sSpriteId != MAX_SPRITES)
    {
        FreeSpriteTilesByTag(TAG_CB_SPRITE_ICON);
        FreeSpritePaletteByTag(TAG_CB_SPRITE_ICON);
        DestroySprite(&gSprites[sSpriteId]);
    }
}

#undef sAuxWindowId
#undef sSpriteId
#undef TAG_CB_SPRITE_ICON

static void FreeListMenuItems(struct ListMenuItem *items, u32 count)
{
    u32 i;
    for (i = 0; i < count; ++i)
    {
        // All items were dynamically allocated, so items[i].name is not actually constant.
        Free((void *)items[i].name);
    }
    Free(items);
}

static u16 UNUSED GetLengthWithExpandedPlayerName(const u8 *str)
{
    u16 length = 0;

    while (*str != EOS)
    {
        if (*str == PLACEHOLDER_BEGIN)
        {
            str++;
            if (*str == PLACEHOLDER_ID_PLAYER)
            {
                length += StringLength(gSaveBlock2Ptr->playerName);
                str++;
            }
        }
        else
        {
            str++;
            length++;
        }
    }

    return length;
}

void MultichoiceDynamic_InitStack(u32 capacity)
{
    AGB_ASSERT(sDynamicMultiChoiceStack == NULL);
    sDynamicMultiChoiceStack = AllocZeroed(sizeof(*sDynamicMultiChoiceStack));
    AGB_ASSERT(sDynamicMultiChoiceStack != NULL);
    sDynamicMultiChoiceStack->capacity = capacity;
    sDynamicMultiChoiceStack->top = -1;
    sDynamicMultiChoiceStack->elements = AllocZeroed(capacity * sizeof(struct ListMenuItem));
}

void MultichoiceDynamic_ReallocStack(u32 newCapacity)
{
    struct ListMenuItem *newElements;
    AGB_ASSERT(sDynamicMultiChoiceStack != NULL);
    AGB_ASSERT(sDynamicMultiChoiceStack->capacity < newCapacity);
    newElements = AllocZeroed(newCapacity * sizeof(struct ListMenuItem));
    AGB_ASSERT(newElements != NULL);
    memcpy(newElements, sDynamicMultiChoiceStack->elements, sDynamicMultiChoiceStack->capacity * sizeof(struct ListMenuItem));
    Free(sDynamicMultiChoiceStack->elements);
    sDynamicMultiChoiceStack->elements = newElements;
    sDynamicMultiChoiceStack->capacity = newCapacity;
}

bool32 MultichoiceDynamic_StackFull(void)
{
    AGB_ASSERT(sDynamicMultiChoiceStack != NULL);
    return sDynamicMultiChoiceStack->top == sDynamicMultiChoiceStack->capacity - 1;
}

bool32 MultichoiceDynamic_StackEmpty(void)
{
    AGB_ASSERT(sDynamicMultiChoiceStack != NULL);
    return sDynamicMultiChoiceStack->top == -1;
}

u32 MultichoiceDynamic_StackSize(void)
{
    AGB_ASSERT(sDynamicMultiChoiceStack != NULL);
    return sDynamicMultiChoiceStack->top + 1;
}

void MultichoiceDynamic_PushElement(struct ListMenuItem item)
{
    if (sDynamicMultiChoiceStack == NULL)
        MultichoiceDynamic_InitStack(MULTICHOICE_DYNAMIC_STACK_SIZE);
    if (MultichoiceDynamic_StackFull())
        MultichoiceDynamic_ReallocStack(sDynamicMultiChoiceStack->capacity + MULTICHOICE_DYNAMIC_STACK_INC);
    sDynamicMultiChoiceStack->elements[++sDynamicMultiChoiceStack->top] = item;
}

struct ListMenuItem *MultichoiceDynamic_PopElement(void)
{
    if (sDynamicMultiChoiceStack == NULL)
        return NULL;
    if (MultichoiceDynamic_StackEmpty())
        return NULL;
    return &sDynamicMultiChoiceStack->elements[sDynamicMultiChoiceStack->top--];
}

struct ListMenuItem *MultichoiceDynamic_PeekElement(void)
{
    if (sDynamicMultiChoiceStack == NULL)
        return NULL;
    if (MultichoiceDynamic_StackEmpty())
        return NULL;
    return &sDynamicMultiChoiceStack->elements[sDynamicMultiChoiceStack->top];
}

struct ListMenuItem *MultichoiceDynamic_PeekElementAt(u32 index)
{
    if (sDynamicMultiChoiceStack == NULL)
        return NULL;
    if (sDynamicMultiChoiceStack->top < index)
        return NULL;
    return &sDynamicMultiChoiceStack->elements[index];
}

void MultichoiceDynamic_DestroyStack(void)
{
    TRY_FREE_AND_SET_NULL(sDynamicMultiChoiceStack->elements);
    TRY_FREE_AND_SET_NULL(sDynamicMultiChoiceStack);
}

static void MultichoiceDynamic_MoveCursor(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u8 taskId;
    if (!onInit)
        PlaySE(SE_SELECT);
    taskId = FindTaskIdByFunc(Task_HandleScrollingMultichoiceInput);
    if (taskId != TASK_NONE)
    {
        ListMenuGetScrollAndRow(gTasks[taskId].data[0], &gScrollableMultichoice_ScrollOffset, NULL);
        if (sDynamicMenuEventId != DYN_MULTICHOICE_CB_NONE && sDynamicListMenuEventCollections[sDynamicMenuEventId].OnSelectionChanged && !onInit)
        {
            struct DynamicListMenuEventArgs eventArgs = {.selectedItem = itemIndex, .windowId = list->template.windowId, .list = &list->template};
            sDynamicListMenuEventCollections[sDynamicMenuEventId].OnSelectionChanged(&eventArgs);
        }
    }
}

static void DrawMultichoiceMenuDynamic(u8 left, u8 top, u8 argc, struct ListMenuItem *items, bool8 ignoreBPress, u32 initialRow, u8 maxBeforeScroll, u32 callbackSet)
{
    u32 i;
    u8 windowId;
    s32 width = 0;
    u8 newWidth;
    u8 taskId;
    u32 windowHeight;
    struct ListMenu *list;

    for (i = 0; i < argc; ++i)
    {
        width = DisplayTextAndGetWidth(items[i].name, width);
    }
    LoadMessageBoxAndBorderGfx();
    windowHeight = (argc < maxBeforeScroll) ? argc * 2 : maxBeforeScroll * 2;
    newWidth = ConvertPixelWidthToTileWidth(width);
    left = ScriptMenu_AdjustLeftCoordFromWidth(left, newWidth);
    windowId = CreateWindowFromRect(left, top, newWidth, windowHeight);
    SetStandardWindowBorderStyle(windowId, FALSE);
    CopyWindowToVram(windowId, COPYWIN_FULL);

    // I don't like this being global either, but I could not come up with another solution that
    // does not invade the whole ListMenu infrastructure.
    sDynamicMenuEventId = callbackSet;
    sDynamicMenuEventScratchPad = AllocZeroed(100 * sizeof(u16));
    if (sDynamicMenuEventId != DYN_MULTICHOICE_CB_NONE && sDynamicListMenuEventCollections[sDynamicMenuEventId].OnInit)
    {
        struct DynamicListMenuEventArgs eventArgs = {.selectedItem = initialRow, .windowId = windowId, .list = NULL};
        sDynamicListMenuEventCollections[sDynamicMenuEventId].OnInit(&eventArgs);
    }

    gMultiuseListMenuTemplate = sScriptableListMenuTemplate;
    gMultiuseListMenuTemplate.windowId = windowId;
    gMultiuseListMenuTemplate.items = items;
    gMultiuseListMenuTemplate.totalItems = argc;
    gMultiuseListMenuTemplate.maxShowed = maxBeforeScroll;
    gMultiuseListMenuTemplate.moveCursorFunc = MultichoiceDynamic_MoveCursor;

    taskId = CreateTask(Task_HandleScrollingMultichoiceInput, 80);
    gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);
    gTasks[taskId].data[1] = ignoreBPress;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[5] = argc;
    gTasks[taskId].data[7] = maxBeforeScroll;
    StoreWordInTwoHalfwords((u16*) &gTasks[taskId].data[3], (u32) items);
    list = (void *) gTasks[gTasks[taskId].data[0]].data;
    ListMenuChangeSelectionFull(list, TRUE, FALSE, initialRow, TRUE);

    if (sDynamicMenuEventId != DYN_MULTICHOICE_CB_NONE && sDynamicListMenuEventCollections[sDynamicMenuEventId].OnSelectionChanged)
    {
        struct DynamicListMenuEventArgs eventArgs = {.selectedItem = items[initialRow].id, .windowId = windowId, .list = &gMultiuseListMenuTemplate};
        sDynamicListMenuEventCollections[sDynamicMenuEventId].OnSelectionChanged(&eventArgs);
    }
    ListMenuGetScrollAndRow(gTasks[taskId].data[0], &gScrollableMultichoice_ScrollOffset, NULL);
    if (argc > maxBeforeScroll)
    {
        // Create Scrolling Arrows
        struct ScrollArrowsTemplate template;
        template.firstX = (newWidth / 2) * 8 + 12 + (left) * 8;
        template.firstY = top * 8 + 5;
        template.secondX = template.firstX;
        template.secondY = top * 8 + windowHeight * 8 + 12;
        template.fullyUpThreshold = 0;
        template.fullyDownThreshold = argc - maxBeforeScroll;
        template.firstArrowType = SCROLL_ARROW_UP;
        template.secondArrowType = SCROLL_ARROW_DOWN;
        template.tileTag = 2000;
        template.palTag = 100,
        template.palNum = 0;

        gTasks[taskId].data[6] = AddScrollIndicatorArrowPair(&template, &gScrollableMultichoice_ScrollOffset);
    }
}

void DrawMultichoiceMenuInternal(u8 left, u8 top, u8 multichoiceId, bool8 ignoreBPress, u8 cursorPos, const struct MenuAction *actions, int count)
{
    int i;
    u8 windowId;
    int width = 0;
    u8 newWidth;

    for (i = 0; i < count; i++)
    {
        width = DisplayTextAndGetWidth(actions[i].text, width);
    }

    newWidth = ConvertPixelWidthToTileWidth(width);
    left = ScriptMenu_AdjustLeftCoordFromWidth(left, newWidth);
    windowId = CreateWindowFromRect(left, top, newWidth, count * 2);
    SetStandardWindowBorderStyle(windowId, TRUE);
    PrintMenuTable(windowId, count, actions);
    InitMenuInUpperLeftCornerNormal(windowId, count, cursorPos);
    ScheduleBgCopyTilemapToVram(0);
    InitMultichoiceCheckWrap(ignoreBPress, count, windowId, multichoiceId);
}

static void DrawMultichoiceMenu(u8 left, u8 top, u8 multichoiceId, bool8 ignoreBPress, u8 cursorPos)
{
    DrawMultichoiceMenuInternal(left, top, multichoiceId, ignoreBPress, cursorPos, sMultichoiceLists[multichoiceId].list, sMultichoiceLists[multichoiceId].count);
}

#define tLeft           data[0]
#define tTop            data[1]
#define tRight          data[2]
#define tBottom         data[3]
#define tIgnoreBPress   data[4]
#define tDoWrap         data[5]
#define tWindowId       data[6]
#define tMultichoiceId  data[7]


/*
 * Adventure Settings use the normal GBA cursor menu, with a compact help
 * panel that changes as the player highlights each choice.
 */
static EWRAM_DATA u8 sAdventureHelpWindow;
static EWRAM_DATA u8 sAdventureHelpCursor;
static EWRAM_DATA bool8 sAdventureHelpActive;

static const u8 sHelp_Gen1[] = _("Kanto: Bulbasaur, Pikachu,\nCharmander and the originals.");
static const u8 sHelp_Gen2[] = _("Johto: Chikorita, Cyndaquil,\nTotodile and new evolutions.");
static const u8 sHelp_Gen3[] = _("Hoenn: Treecko, Torchic,\nMudkip and many new species.");
static const u8 sHelp_Gen4[] = _("Sinnoh: Turtwig, Chimchar,\nPiplup and new evolutions.");
static const u8 sHelp_Gen5[] = _("Unova: Snivy, Tepig,\nOshawott and a fresh roster.");
static const u8 sHelp_Gen6[] = _("Kalos: Chespin, Fennekin,\nFroakie and Fairy-type debut.");
static const u8 sHelp_Gen7[] = _("Alola: Rowlet, Litten,\nPopplio and regional forms.");
static const u8 sHelp_Gen8[] = _("Galar: Grookey, Scorbunny,\nSobble and new adventures.");
static const u8 sHelp_Gen9[] = _("Paldea: Sprigatito, Fuecoco,\nQuaxly and new discoveries.");
static const u8 sHelp_All[] = _("All: Pokemon from every\ngeneration may appear.");
static const u8 sHelp_More[] = _("See generations 6-9 or\nchoose every generation.");
static const u8 sHelp_Only[] = _("ONLY: Just Pokemon first\nintroduced in your chosen gen.");
static const u8 sHelp_Unlocked[] = _("UNLOCKED: Your chosen gen\nplus every earlier generation.");
static const u8 sHelp_Modern[] = _("MODERN: All party Pokemon\ngain EXP after a battle.");
static const u8 sHelp_Classic[] = _("CLASSIC: Participants and\nExp. Share holders gain EXP.");
static const u8 sHelp_Off[] = _("OFF: Only battle participants\ngain EXP. No party sharing.");
static const u8 sHelp_SelectGen[] = _("Pick the Pokemon generation\nfor this adventure.");
static const u8 sHelp_SelectExp[] = _("Choose how EXP is shared.\nYou can change this later.");
static const u8 sHelp_Exit[] = _("Return to your room. Both\nsettings are needed to leave.");

static const u8 *const sAdventureGenFirstHelp[] = {sHelp_Gen1, sHelp_Gen2, sHelp_Gen3, sHelp_Gen4, sHelp_Gen5, sHelp_More};
static const u8 *const sAdventureGenSecondHelp[] = {sHelp_Gen6, sHelp_Gen7, sHelp_Gen8, sHelp_Gen9, sHelp_All};
static const u8 *const sAdventureModeHelp[] = {sHelp_Only, sHelp_Unlocked};
static const u8 *const sAdventureExpHelp[] = {sHelp_Modern, sHelp_Classic, sHelp_Off};

static bool8 IsAdventureMultichoice(u8 id)
{
    return id == MULTI_ADVENTURE_SETTINGS
        || id == MULTI_ADVENTURE_GEN_SECOND || id == MULTI_ADVENTURE_GEN_MODE
        || id == MULTI_ADVENTURE_EXP_SHARE;
}

static void ApplyDeluxeMenuPalette(void);
static void RestoreDeluxeMenuPalette(void);

static void UpdateAdventureHelp(u8 id)
{
    u8 cursor = Menu_GetCursorPos();
    const u8 *message = sHelp_Exit;

    if (!sAdventureHelpActive || cursor == sAdventureHelpCursor)
        return;
    sAdventureHelpCursor = cursor;

    if (id == MULTI_ADVENTURE_GEN_FIRST && cursor < ARRAY_COUNT(sAdventureGenFirstHelp))
        message = sAdventureGenFirstHelp[cursor];
    else if (id == MULTI_ADVENTURE_GEN_SECOND && cursor < ARRAY_COUNT(sAdventureGenSecondHelp))
        message = sAdventureGenSecondHelp[cursor];
    else if (id == MULTI_ADVENTURE_GEN_MODE && cursor < ARRAY_COUNT(sAdventureModeHelp))
        message = sAdventureModeHelp[cursor];
    else if (id == MULTI_ADVENTURE_EXP_SHARE && cursor < ARRAY_COUNT(sAdventureExpHelp))
        message = sAdventureExpHelp[cursor];
    else if (id == MULTI_ADVENTURE_SETTINGS)
    {
        if (cursor == 0)
            message = sHelp_SelectGen;
        else if (cursor == 1)
            message = sHelp_SelectExp;
    }

    FillWindowPixelBuffer(sAdventureHelpWindow, PIXEL_FILL(1));
    AddTextPrinterParameterized(sAdventureHelpWindow, FONT_NORMAL, message, 8, 2, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sAdventureHelpWindow, COPYWIN_FULL);
}

static void CloseAdventureHelp(void)
{
    if (sAdventureHelpActive)
    {
        ClearToTransparentAndRemoveWindow(sAdventureHelpWindow);
        RestoreDeluxeMenuPalette();
        sAdventureHelpActive = FALSE;
    }
    sAdventureHelpCursor = 0xFF;
}


/*
 * Bag-style, scrollable generation checklist.
 * Five visible rows, nine independent toggles, All/Clear and Done.
 * The selection is only saved when Done is chosen with a nonempty mask.
 */
#define GEN_CHECKLIST_ITEMS 12
#define GEN_CHECKLIST_VISIBLE 5
#define DELUXE_THEME_VAR VAR_UNUSED_0x40FE

// Recolor only the menu palette, never the overworld or battle palettes.
static u16 sDeluxeSavedPalette[16];
static bool8 sDeluxePaletteActive;

static void ApplyDeluxeMenuPalette(void)
{
    u16 colors[16];
    u8 i;
    if (!sDeluxePaletteActive)
    {
        for (i = 0; i < 16; i++)
            sDeluxeSavedPalette[i] = gPlttBufferUnfaded[BG_PLTT_ID(15) + i];
        sDeluxePaletteActive = TRUE;
    }
    for (i = 0; i < 16; i++)
        colors[i] = sDeluxeSavedPalette[i];

    // Palette indices 1-7 are local to the menu windows (BG palette 15).
    // Text uses index 2 for foreground and 3 for its shadow.
    if (VarGet(DELUXE_THEME_VAR) == 1)
    {
        colors[1] = RGB(5, 6, 9);    // Charcoal panel
        colors[2] = RGB(31, 29, 25); // Light text
        colors[3] = RGB(13, 12, 11); // Text shadow
        colors[4] = RGB(18, 5, 7);   // Muted red header
        colors[5] = RGB(31, 30, 26); // Header lettering
        colors[6] = RGB(11, 12, 16); // Highlight row
        colors[7] = RGB(24, 18, 8);  // Warm gold
    }
    else
    {
        colors[1] = RGB(31, 30, 26); // Cream panel
        colors[2] = RGB(8, 6, 8);    // Ink lettering
        colors[3] = RGB(24, 19, 12); // Text shadow
        colors[4] = RGB(24, 4, 7);   // FireRed header
        colors[5] = RGB(31, 31, 29); // Header lettering
        colors[6] = RGB(31, 28, 19); // Gold-tinted selected row
        colors[7] = RGB(23, 16, 5);  // Antique gold
    }
    LoadPalette(colors, BG_PLTT_ID(15), sizeof(colors));
}

static void RestoreDeluxeMenuPalette(void)
{
    if (sDeluxePaletteActive)
    {
        LoadPalette(sDeluxeSavedPalette, BG_PLTT_ID(15), sizeof(sDeluxeSavedPalette));
        sDeluxePaletteActive = FALSE;
    }
}

static const u8 sDeluxeHeaderTextColors[] = {4, 5, 7};
static const u8 *const sDeluxeSelectedCount[] =
{
    COMPOUND_STRING("0/9"), COMPOUND_STRING("1/9"), COMPOUND_STRING("2/9"),
    COMPOUND_STRING("3/9"), COMPOUND_STRING("4/9"), COMPOUND_STRING("5/9"),
    COMPOUND_STRING("6/9"), COMPOUND_STRING("7/9"), COMPOUND_STRING("8/9"),
    COMPOUND_STRING("9/9"),
};
#define tGenListWindow data[0]
#define tGenHelpWindow data[1]
#define tGenCursor data[2]
#define tGenScroll data[3]
#define tGenMask data[4]
#define tGenNeedsSelection data[5]

static const u8 *const sGenerationChecklistLabels[] =
{
    COMPOUND_STRING("GEN 1  KANTO"),
    COMPOUND_STRING("GEN 2  JOHTO"),
    COMPOUND_STRING("GEN 3  HOENN"),
    COMPOUND_STRING("GEN 4  SINNOH"),
    COMPOUND_STRING("GEN 5  UNOVA"),
    COMPOUND_STRING("GEN 6  KALOS"),
    COMPOUND_STRING("GEN 7  ALOLA"),
    COMPOUND_STRING("GEN 8  GALAR"),
    COMPOUND_STRING("GEN 9  PALDEA"),
    COMPOUND_STRING("SELECT ALL"),
    COMPOUND_STRING("CLEAR ALL"),
    COMPOUND_STRING("DONE"),
};

static const u8 *const sGenerationChecklistHelp[] =
{
    sHelp_Gen1, sHelp_Gen2, sHelp_Gen3,
    sHelp_Gen4, sHelp_Gen5, sHelp_Gen6,
    sHelp_Gen7, sHelp_Gen8, sHelp_Gen9,
    COMPOUND_STRING("Check every generation."),
    COMPOUND_STRING("Uncheck every generation."),
    COMPOUND_STRING("Save checked generations.\nAt least one is required."),
};

static void DrawGenerationChecklist(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i, index, selectedCount = 0;
    const u8 *check;
    const u8 *cursor;

    for (i = 0; i < 9; i++)
        if (tGenMask & (1 << i))
            selectedCount++;

    // Deliberately simple, readable 240x160 GBA layout. No patterned
    // outer frame or overlapping text in the header.
    FillWindowPixelBuffer(tGenListWindow, PIXEL_FILL(1));
    FillWindowPixelRect(tGenListWindow, PIXEL_FILL(7), 0, 0, 240, 2);
    FillWindowPixelRect(tGenListWindow, PIXEL_FILL(4), 0, 2, 240, 18);
    FillWindowPixelRect(tGenListWindow, PIXEL_FILL(7), 0, 20, 240, 2);
    AddTextPrinterParameterized3(tGenListWindow, FONT_NORMAL, 8, 2,
        sDeluxeHeaderTextColors, TEXT_SKIP_DRAW, COMPOUND_STRING("GENERATIONS"));
    AddTextPrinterParameterized3(tGenListWindow, FONT_NORMAL, 201, 2,
        sDeluxeHeaderTextColors, TEXT_SKIP_DRAW, sDeluxeSelectedCount[selectedCount]);

    for (i = 0; i < GEN_CHECKLIST_VISIBLE; i++)
    {
        index = tGenScroll + i;
        if (index >= GEN_CHECKLIST_ITEMS)
            break;

        if (index == tGenCursor)
            FillWindowPixelRect(tGenListWindow, PIXEL_FILL(6), 3, 24 + i * 14, 234, 14);

        cursor = index == tGenCursor ? COMPOUND_STRING(">") : COMPOUND_STRING(" ");
        AddTextPrinterParameterized(tGenListWindow, FONT_NORMAL,
            cursor, 8, 24 + i * 14, TEXT_SKIP_DRAW, NULL);

        if (index < 9)
        {
            check = (tGenMask & (1 << index))
                ? COMPOUND_STRING("X") : COMPOUND_STRING(" ");
            FillWindowPixelRect(tGenListWindow, PIXEL_FILL(7), 27, 26 + i * 14, 11, 11);
            FillWindowPixelRect(tGenListWindow, PIXEL_FILL(1), 28, 27 + i * 14, 9, 9);
            AddTextPrinterParameterized(tGenListWindow, FONT_NORMAL,
                check, 28, 24 + i * 14, TEXT_SKIP_DRAW, NULL);
        }
        AddTextPrinterParameterized(tGenListWindow, FONT_NORMAL,
            sGenerationChecklistLabels[index], index < 9 ? 48 : 28,
            24 + i * 14, TEXT_SKIP_DRAW, NULL);
    }
    FillWindowPixelRect(tGenListWindow, PIXEL_FILL(7), 0, 115, 240, 2);
    CopyWindowToVram(tGenListWindow, COPYWIN_FULL);

    FillWindowPixelBuffer(tGenHelpWindow, PIXEL_FILL(1));
    FillWindowPixelRect(tGenHelpWindow, PIXEL_FILL(7), 0, 0, 240, 2);
    if (tGenNeedsSelection)
        AddTextPrinterParameterized(tGenHelpWindow, FONT_NORMAL,
            COMPOUND_STRING("Choose at least one gen!\nPress A to check a box."),
            6, 3, TEXT_SKIP_DRAW, NULL);
    else
        AddTextPrinterParameterized(tGenHelpWindow, FONT_NORMAL,
            sGenerationChecklistHelp[tGenCursor], 6, 3, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(tGenHelpWindow, COPYWIN_FULL);
}

static void CloseGenerationChecklist(u8 taskId, bool8 save)
{
    s16 *data = gTasks[taskId].data;

    if (save)
    {
        SetGenerationSelectionMask(tGenMask);
        VarSet(VAR_ADVENTURE_GEN_CHOSEN, 1);
        gSpecialVar_Result = 0;
    }
    else
    {
        gSpecialVar_Result = MULTI_B_PRESSED;
    }

    RestoreDeluxeMenuPalette();
    ClearToTransparentAndRemoveWindow(tGenHelpWindow);
    ClearToTransparentAndRemoveWindow(tGenListWindow);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_GenerationChecklist(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    bool8 redraw = FALSE;

    if (gPaletteFade.active)
        return;

    if (JOY_NEW(DPAD_UP))
    {
        tGenCursor = (tGenCursor + GEN_CHECKLIST_ITEMS - 1) % GEN_CHECKLIST_ITEMS;
        redraw = TRUE;
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        tGenCursor = (tGenCursor + 1) % GEN_CHECKLIST_ITEMS;
        redraw = TRUE;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        CloseGenerationChecklist(taskId, FALSE);
        return;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (tGenCursor < 9)
            tGenMask ^= 1 << tGenCursor;
        else if (tGenCursor == 9)
            tGenMask = 0x01FF;
        else if (tGenCursor == 10)
            tGenMask = 0;
        else if (tGenMask != 0)
        {
            CloseGenerationChecklist(taskId, TRUE);
            return;
        }
        else
            tGenNeedsSelection = TRUE;
        redraw = TRUE;
    }

    if (redraw)
    {
        if (tGenCursor < tGenScroll)
            tGenScroll = tGenCursor;
        if (tGenCursor >= tGenScroll + GEN_CHECKLIST_VISIBLE)
            tGenScroll = tGenCursor - GEN_CHECKLIST_VISIBLE + 1;
        if (tGenCursor != 11)
            tGenNeedsSelection = FALSE;
        DrawGenerationChecklist(taskId);
    }
}

static void OpenGenerationChecklist(void)
{
    struct WindowTemplate listTemplate = CreateWindowTemplate(0, 0, 0, 30, 15, 15, 100);
    struct WindowTemplate helpTemplate = CreateWindowTemplate(0, 0, 15, 30, 5, 15, 600);
    u8 taskId = CreateTask(Task_GenerationChecklist, 80);
    s16 *data = gTasks[taskId].data;

    tGenListWindow = AddWindow(&listTemplate);
    tGenHelpWindow = AddWindow(&helpTemplate);
    PutWindowTilemap(tGenListWindow);
    PutWindowTilemap(tGenHelpWindow);
    ApplyDeluxeMenuPalette();
    tGenCursor = 0;
    tGenScroll = 0;
    tGenMask = GetGenerationSelectionMask();
    tGenNeedsSelection = FALSE;
    DrawGenerationChecklist(taskId);
}
#undef tGenListWindow
#undef tGenHelpWindow
#undef tGenCursor
#undef tGenScroll
#undef tGenMask
#undef tGenNeedsSelection

// Full-width EXP menu: separate task keeps the original script result values.
#define tExpList data[0]
#define tExpHelp data[1]
#define tExpCursor data[2]
static const u8 *const sDeluxeExpNames[] =
{
    COMPOUND_STRING("MODERN"),
    COMPOUND_STRING("CLASSIC"),
    COMPOUND_STRING("OFF"),
};
static const u8 *const sDeluxeExpDetails[] =
{
    COMPOUND_STRING("All party Pokemon earn EXP.\nEasy team training."),
    COMPOUND_STRING("Battle participants and\nExp. Share holders earn EXP."),
    COMPOUND_STRING("Only battle participants\nearn EXP. No party sharing."),
};

static void DrawDeluxeExpMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i;
    FillWindowPixelBuffer(tExpList, PIXEL_FILL(1));
    FillWindowPixelRect(tExpList, PIXEL_FILL(7), 0, 0, 240, 2);
    FillWindowPixelRect(tExpList, PIXEL_FILL(4), 0, 2, 240, 18);
    FillWindowPixelRect(tExpList, PIXEL_FILL(7), 0, 20, 240, 2);
    AddTextPrinterParameterized3(tExpList, FONT_NORMAL, 8, 2,
        sDeluxeHeaderTextColors, TEXT_SKIP_DRAW, COMPOUND_STRING("EXP SHARE MODE"));
    for (i = 0; i < 3; i++)
    {
        u8 y = 28 + i * 20;
        if (tExpCursor == i)
            FillWindowPixelRect(tExpList, PIXEL_FILL(6), 4, y, 232, 18);
        // Radio-style checkmark: only the highlighted EXP mode is active.
        AddTextPrinterParameterized(tExpList, FONT_NORMAL,
            i == tExpCursor ? COMPOUND_STRING(">") : COMPOUND_STRING(" "),
            10, y, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(tExpList, FONT_NORMAL,
            i == tExpCursor ? COMPOUND_STRING("[X]") : COMPOUND_STRING("[ ]"),
            29, y, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(tExpList, FONT_NORMAL,
            sDeluxeExpNames[i], 65, y, TEXT_SKIP_DRAW, NULL);
    }
    FillWindowPixelRect(tExpList, PIXEL_FILL(7), 0, 115, 240, 2);
    CopyWindowToVram(tExpList, COPYWIN_FULL);

    FillWindowPixelBuffer(tExpHelp, PIXEL_FILL(1));
    FillWindowPixelRect(tExpHelp, PIXEL_FILL(7), 0, 0, 240, 2);
    AddTextPrinterParameterized(tExpHelp, FONT_NORMAL,
        sDeluxeExpDetails[tExpCursor], 8, 3, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(tExpHelp, COPYWIN_FULL);
}

static void Task_DeluxeExpMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (gPaletteFade.active)
        return;
    if (JOY_NEW(DPAD_UP))
        tExpCursor = (tExpCursor + 2) % 3;
    else if (JOY_NEW(DPAD_DOWN))
        tExpCursor = (tExpCursor + 1) % 3;
    else if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        gSpecialVar_Result = JOY_NEW(B_BUTTON) ? MULTI_B_PRESSED : tExpCursor;
        ClearToTransparentAndRemoveWindow(tExpHelp);
        ClearToTransparentAndRemoveWindow(tExpList);
        RestoreDeluxeMenuPalette();
        DestroyTask(taskId);
        ScriptContext_Enable();
        return;
    }
    else
        return;
    PlaySE(SE_SELECT);
    DrawDeluxeExpMenu(taskId);
}

static void OpenDeluxeExpMenu(u8 defaultChoice)
{
    struct WindowTemplate listTemplate = CreateWindowTemplate(0, 0, 0, 30, 15, 15, 100);
    struct WindowTemplate helpTemplate = CreateWindowTemplate(0, 0, 15, 30, 5, 15, 600);
    u8 taskId = CreateTask(Task_DeluxeExpMenu, 80);
    s16 *data = gTasks[taskId].data;
    tExpList = AddWindow(&listTemplate);
    tExpHelp = AddWindow(&helpTemplate);
    PutWindowTilemap(tExpList);
    PutWindowTilemap(tExpHelp);
    tExpCursor = defaultChoice < 3 ? defaultChoice : 0;
    ApplyDeluxeMenuPalette();
    DrawDeluxeExpMenu(taskId);
}
#undef tExpList
#undef tExpHelp
#undef tExpCursor

static void InitMultichoiceCheckWrap(bool8 ignoreBPress, u8 count, u8 windowId, u8 multichoiceId)
{
    u8 i;
    u8 taskId;
    sProcessInputDelay = 2;

    for (i = 0; i < ARRAY_COUNT(sLinkServicesMultichoiceIds); i++)
    {
        if (sLinkServicesMultichoiceIds[i] == multichoiceId)
        {
            sProcessInputDelay = 12;
        }
    }

    taskId = CreateTask(Task_HandleMultichoiceInput, 80);

    gTasks[taskId].tIgnoreBPress = ignoreBPress;

    if (count > 3)
        gTasks[taskId].tDoWrap = TRUE;
    else
        gTasks[taskId].tDoWrap = FALSE;

    gTasks[taskId].tWindowId = windowId;
    gTasks[taskId].tMultichoiceId = multichoiceId;

    DrawLinkServicesMultichoiceMenu(multichoiceId);
    if (IsAdventureMultichoice(multichoiceId))
    {
        CloseAdventureHelp();
        // Keep the help panel in separate BG tile memory.
        {
            struct WindowTemplate helpTemplate = CreateWindowTemplate(0, 2, 15, 26, 4, 15, 600);
            sAdventureHelpWindow = AddWindow(&helpTemplate);
            PutWindowTilemap(sAdventureHelpWindow);
        }
        sAdventureHelpActive = TRUE;
        SetStandardWindowBorderStyle(sAdventureHelpWindow, TRUE);
        ApplyDeluxeMenuPalette();
        UpdateAdventureHelp(multichoiceId);
    }
}

static void Task_HandleScrollingMultichoiceInput(u8 taskId)
{
    bool32 done = FALSE;
    s32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    switch (input)
    {
    case LIST_HEADER:
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        if (!gTasks[taskId].data[1])
        {
            gSpecialVar_Result = MULTI_B_PRESSED;
            done = TRUE;
        }
        break;
    default:
        gSpecialVar_Result = input;
        done = TRUE;
        break;
    }

    if (done)
    {
        struct ListMenuItem *items;

        PlaySE(SE_SELECT);

        if (sDynamicMenuEventId != DYN_MULTICHOICE_CB_NONE && sDynamicListMenuEventCollections[sDynamicMenuEventId].OnDestroy)
        {
            struct DynamicListMenuEventArgs eventArgs = {.selectedItem = input, .windowId = gTasks[taskId].data[2], .list = NULL};
            sDynamicListMenuEventCollections[sDynamicMenuEventId].OnDestroy(&eventArgs);
        }

        sDynamicMenuEventId = DYN_MULTICHOICE_CB_NONE;

        if (gTasks[taskId].data[5] > gTasks[taskId].data[7])
        {
            RemoveScrollIndicatorArrowPair(gTasks[taskId].data[6]);
        }

        LoadWordFromTwoHalfwords((u16*) &gTasks[taskId].data[3], (u32* )(&items));
        FreeListMenuItems(items, gTasks[taskId].data[5]);
        TRY_FREE_AND_SET_NULL(sDynamicMenuEventScratchPad);
        DestroyListMenuTask(gTasks[taskId].data[0], NULL, NULL);
        ClearStdWindowAndFrame(gTasks[taskId].data[2], TRUE);
        RemoveWindow(gTasks[taskId].data[2]);
        ScriptContext_Enable();
        DestroyTask(taskId);
    }
}

static void Task_HandleMultichoiceInput(u8 taskId)
{
    s8 selection;
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        if (sProcessInputDelay)
        {
            sProcessInputDelay--;
        }
        else
        {
            if (!tDoWrap)
                selection = Menu_ProcessInputNoWrap();
            else
                selection = Menu_ProcessInput();

            if (JOY_NEW(DPAD_UP | DPAD_DOWN))
            {
                DrawLinkServicesMultichoiceMenu(tMultichoiceId);
                if (IsAdventureMultichoice(tMultichoiceId))
                    UpdateAdventureHelp(tMultichoiceId);
            }

            if (selection != MENU_NOTHING_CHOSEN)
            {
                if (selection == MENU_B_PRESSED)
                {
                    if (tIgnoreBPress)
                        return;
                    PlaySE(SE_SELECT);
                    gSpecialVar_Result = MULTI_B_PRESSED;
                }
                else
                {
                    gSpecialVar_Result = selection;
                }
                if (IsAdventureMultichoice(tMultichoiceId))
                    CloseAdventureHelp();
                ClearToTransparentAndRemoveWindow(tWindowId);
                DestroyTask(taskId);
                ScriptContext_Enable();
            }
        }
    }
}

bool8 ScriptMenu_YesNo(u8 left, u8 top)
{
    if (FuncIsActiveTask(Task_HandleYesNoInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        DisplayYesNoMenuDefaultYes();
        CreateTask(Task_HandleYesNoInput, 0x50);
        return TRUE;
    }
}

static void Task_HandleYesNoInput(u8 taskId)
{
    if (gTasks[taskId].tRight < 5)
    {
        gTasks[taskId].tRight++;
        return;
    }

    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case MENU_NOTHING_CHOSEN:
        return;
    case MENU_B_PRESSED:
    case 1:
        PlaySE(SE_SELECT);
        gSpecialVar_Result = 0;
        break;
    case 0:
        gSpecialVar_Result = 1;
        break;
    }

    DestroyTask(taskId);
    ScriptContext_Enable();
}

bool8 ScriptMenu_MultichoiceGrid(u8 left, u8 top, u8 multichoiceId, bool8 ignoreBPress, u8 columnCount)
{
    if (FuncIsActiveTask(Task_HandleMultichoiceGridInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        u8 taskId;
        u8 rowCount, newWidth;
        int i, width;

        gSpecialVar_Result = 0xFF;
        width = 0;

        for (i = 0; i < sMultichoiceLists[multichoiceId].count; i++)
        {
            width = DisplayTextAndGetWidth(sMultichoiceLists[multichoiceId].list[i].text, width);
        }

        newWidth = ConvertPixelWidthToTileWidth(width);

        left = ScriptMenu_AdjustLeftCoordFromWidth(left, columnCount * newWidth);
        rowCount = sMultichoiceLists[multichoiceId].count / columnCount;

        taskId = CreateTask(Task_HandleMultichoiceGridInput, 80);

        gTasks[taskId].tIgnoreBPress = ignoreBPress;
        gTasks[taskId].tWindowId = CreateWindowFromRect(left, top, columnCount * newWidth, rowCount * 2);
        SetStandardWindowBorderStyle(gTasks[taskId].tWindowId, FALSE);
        PrintMenuGridTable(gTasks[taskId].tWindowId, newWidth * 8, columnCount, rowCount, sMultichoiceLists[multichoiceId].list);
        InitMenuActionGrid(gTasks[taskId].tWindowId, newWidth * 8, columnCount, rowCount, 0);
        CopyWindowToVram(gTasks[taskId].tWindowId, COPYWIN_FULL);
        return TRUE;
    }
}

static void Task_HandleMultichoiceGridInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s8 selection = Menu_ProcessGridInput();

    switch (selection)
    {
    case MENU_NOTHING_CHOSEN:
        return;
    case MENU_B_PRESSED:
        if (tIgnoreBPress)
            return;
        PlaySE(SE_SELECT);
        gSpecialVar_Result = MULTI_B_PRESSED;
        break;
    default:
        gSpecialVar_Result = selection;
        break;
    }

    ClearToTransparentAndRemoveWindow(tWindowId);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

#undef tWindowId

bool16 ScriptMenu_CreatePCMultichoice(void)
{
    if (FuncIsActiveTask(Task_HandleMultichoiceInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        CreatePCMultichoice();
        return TRUE;
    }
}

static void CreatePCMultichoice(void)
{
    u8 x = 8;
    u32 pixelWidth = 0;
    u8 width;
    u8 numChoices;
    u8 windowId;
    int i;

    for (i = 0; i < ARRAY_COUNT(sPCNameStrings); i++)
    {
        pixelWidth = DisplayTextAndGetWidth(sPCNameStrings[i], pixelWidth);
    }

    if (FlagGet(FLAG_SYS_GAME_CLEAR))
    {
        pixelWidth = DisplayTextAndGetWidth(gText_HallOfFame, pixelWidth);
    }

    width = ConvertPixelWidthToTileWidth(pixelWidth);

    // Include Hall of Fame option if player is champion
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
    {
        numChoices = 4;
        windowId = CreateWindowFromRect(0, 0, width, 8);
        SetStandardWindowBorderStyle(windowId, FALSE);
        AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_HallOfFame, x, 33, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_LogOff, x, 49, TEXT_SKIP_DRAW, NULL);
    }
    else
    {
        numChoices = 3;
        windowId = CreateWindowFromRect(0, 0, width, 6);
        SetStandardWindowBorderStyle(windowId, FALSE);
        AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_LogOff, x, 33, TEXT_SKIP_DRAW, NULL);
    }

    // Change PC name if player has met Lanette
    if (FlagGet(FLAG_SYS_PC_LANETTE))
    {
        if (IS_FRLG)
            AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_BillsPc, x, 1, TEXT_SKIP_DRAW, NULL);
        else
            AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_LanettesPC, x, 1, TEXT_SKIP_DRAW, NULL);
    }
    else
    {
        AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_SomeonesPC, x, 1, TEXT_SKIP_DRAW, NULL);
    }

    StringExpandPlaceholders(gStringVar4, gText_PlayersPC);
    PrintPlayerNameOnWindow(windowId, gStringVar4, x, 17);
    InitMenuInUpperLeftCornerNormal(windowId, numChoices, 0);
    CopyWindowToVram(windowId, COPYWIN_FULL);
    InitMultichoiceCheckWrap(FALSE, numChoices, windowId, MULTI_PC);
}

void ScriptMenu_DisplayPCStartupPrompt(void)
{
    LoadMessageBoxAndFrameGfx(0, TRUE);
    AddTextPrinterParameterized2(0, FONT_NORMAL, gText_WhichPCShouldBeAccessed, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
}

bool8 ScriptMenu_CreateLilycoveSSTidalMultichoice(void)
{
    if (FuncIsActiveTask(Task_HandleMultichoiceInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        CreateLilycoveSSTidalMultichoice();
        return TRUE;
    }
}

// gSpecialVar_0x8004 is 1 if the Sailor was shown multiple event tickets at the same time
// otherwise gSpecialVar_0x8004 is 0
static void CreateLilycoveSSTidalMultichoice(void)
{
    u8 selectionCount = 0;
    u8 count;
    u32 pixelWidth;
    u8 width;
    u8 windowId;
    u8 i;
    u32 j;

    for (i = 0; i < SSTIDAL_SELECTION_COUNT; i++)
    {
        sLilycoveSSTidalSelections[i] = 0xFF;
    }

    GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_WIDTH);

    if (gSpecialVar_0x8004 == 0)
    {
        sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_SLATEPORT;
        selectionCount++;

        if (FlagGet(FLAG_MET_SCOTT_ON_SS_TIDAL) == TRUE)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_BATTLE_FRONTIER;
            selectionCount++;
        }
    }

    if (CheckBagHasItem(ITEM_EON_TICKET, 1) == TRUE && FlagGet(FLAG_ENABLE_SHIP_SOUTHERN_ISLAND) == TRUE)
    {
        if (gSpecialVar_0x8004 == 0)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_SOUTHERN_ISLAND;
            selectionCount++;
        }

        if (gSpecialVar_0x8004 == 1 && FlagGet(FLAG_SHOWN_EON_TICKET) == FALSE)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_SOUTHERN_ISLAND;
            selectionCount++;
            FlagSet(FLAG_SHOWN_EON_TICKET);
        }
    }

    if (CheckBagHasItem(ITEM_MYSTIC_TICKET, 1) == TRUE && FlagGet(FLAG_ENABLE_SHIP_NAVEL_ROCK) == TRUE)
    {
        if (gSpecialVar_0x8004 == 0)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_NAVEL_ROCK;
            selectionCount++;
        }

        if (gSpecialVar_0x8004 == 1 && FlagGet(FLAG_SHOWN_MYSTIC_TICKET) == FALSE)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_NAVEL_ROCK;
            selectionCount++;
            FlagSet(FLAG_SHOWN_MYSTIC_TICKET);
        }
    }

    if (CheckBagHasItem(ITEM_AURORA_TICKET, 1) == TRUE && FlagGet(FLAG_ENABLE_SHIP_BIRTH_ISLAND) == TRUE)
    {
        if (gSpecialVar_0x8004 == 0)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_BIRTH_ISLAND;
            selectionCount++;
        }

        if (gSpecialVar_0x8004 == 1 && FlagGet(FLAG_SHOWN_AURORA_TICKET) == FALSE)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_BIRTH_ISLAND;
            selectionCount++;
            FlagSet(FLAG_SHOWN_AURORA_TICKET);
        }
    }

    if (CheckBagHasItem(ITEM_OLD_SEA_MAP, 1) == TRUE && FlagGet(FLAG_ENABLE_SHIP_FARAWAY_ISLAND) == TRUE)
    {
        if (gSpecialVar_0x8004 == 0)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_FARAWAY_ISLAND;
            selectionCount++;
        }

        if (gSpecialVar_0x8004 == 1 && FlagGet(FLAG_SHOWN_OLD_SEA_MAP) == FALSE)
        {
            sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_FARAWAY_ISLAND;
            selectionCount++;
            FlagSet(FLAG_SHOWN_OLD_SEA_MAP);
        }
    }

    sLilycoveSSTidalSelections[selectionCount] = SSTIDAL_SELECTION_EXIT;
    selectionCount++;

    if (gSpecialVar_0x8004 == 0 && FlagGet(FLAG_MET_SCOTT_ON_SS_TIDAL) == TRUE)
    {
        count = selectionCount;
    }

    count = selectionCount;
    if (count == SSTIDAL_SELECTION_COUNT)
    {
        gSpecialVar_0x8004 = SCROLL_MULTI_SS_TIDAL_DESTINATION;
        ShowScrollableMultichoice();
    }
    else
    {
        pixelWidth = 0;

        for (j = 0; j < SSTIDAL_SELECTION_COUNT; j++)
        {
            u8 selection = sLilycoveSSTidalSelections[j];
            if (selection != 0xFF)
            {
                pixelWidth = DisplayTextAndGetWidth(sLilycoveSSTidalDestinations[selection], pixelWidth);
            }
        }

        width = ConvertPixelWidthToTileWidth(pixelWidth);
        windowId = CreateWindowFromRect(MAX_MULTICHOICE_WIDTH - width, (6 - count) * 2, width, count * 2);
        SetStandardWindowBorderStyle(windowId, FALSE);

        for (selectionCount = 0, i = 0; i < SSTIDAL_SELECTION_COUNT; i++)
        {
            if (sLilycoveSSTidalSelections[i] != 0xFF)
            {
                AddTextPrinterParameterized(windowId, FONT_NORMAL, sLilycoveSSTidalDestinations[sLilycoveSSTidalSelections[i]], 8, selectionCount * 16 + 1, TEXT_SKIP_DRAW, NULL);
                selectionCount++;
            }
        }

        InitMenuInUpperLeftCornerNormal(windowId, count, count - 1);
        CopyWindowToVram(windowId, COPYWIN_FULL);
        InitMultichoiceCheckWrap(FALSE, count, windowId, MULTI_SSTIDAL_LILYCOVE);
    }
}

void GetLilycoveSSTidalSelection(void)
{
    if (gSpecialVar_Result != MULTI_B_PRESSED)
    {
        gSpecialVar_Result = sLilycoveSSTidalSelections[gSpecialVar_Result];
    }
}

#define tState       data[0]
#define tMonSpecies  data[1]
#define tMonSpriteId data[2]
#define tWindowX     data[3]
#define tWindowY     data[4]
#define tWindowId    data[5]

static void Task_PokemonPicWindow(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case 0:
        task->tState++;
        break;
    case 1:
        // Wait until state is advanced by ScriptMenu_HidePokemonPic
        break;
    case 2:
        FreeResourcesAndDestroySprite(&gSprites[task->tMonSpriteId], task->tMonSpriteId);
        task->tState++;
        break;
    case 3:
        ClearToTransparentAndRemoveWindow(task->tWindowId);
        DestroyTask(taskId);
        break;
    }
}

bool8 ScriptMenu_ShowPokemonPic(enum Species species, u8 x, u8 y)
{
    u8 taskId;
    u8 spriteId;

    if (FindTaskIdByFunc(Task_PokemonPicWindow) != TASK_NONE)
    {
        return FALSE;
    }
    else
    {
        spriteId = CreateMonSprite_PicBox(species, x * 8 + 40, y * 8 + 40, 0);
        taskId = CreateTask(Task_PokemonPicWindow, 0x50);
        gTasks[taskId].tWindowId = CreateWindowFromRect(x, y, 8, 8);
        gTasks[taskId].tState = 0;
        gTasks[taskId].tMonSpecies = species;
        gTasks[taskId].tMonSpriteId = spriteId;
        gSprites[spriteId].callback = SpriteCallbackDummy;
        gSprites[spriteId].oam.priority = 0;
        SetStandardWindowBorderStyle(gTasks[taskId].tWindowId, TRUE);
        ScheduleBgCopyTilemapToVram(0);
        return TRUE;
    }
}

bool8 (*ScriptMenu_HidePokemonPic(void))(void)
{
    u8 taskId = FindTaskIdByFunc(Task_PokemonPicWindow);

    if (taskId == TASK_NONE)
        return NULL;
    gTasks[taskId].tState++;
    return IsPicboxClosed;
}

static bool8 IsPicboxClosed(void)
{
    if (FindTaskIdByFunc(Task_PokemonPicWindow) == TASK_NONE)
        return TRUE;
    else
        return FALSE;
}

#undef tState
#undef tMonSpecies
#undef tMonSpriteId
#undef tWindowX
#undef tWindowY
#undef tWindowId

u8 CreateWindowFromRect(u8 x, u8 y, u8 width, u8 height)
{
    struct WindowTemplate template = CreateWindowTemplate(0, x + 1, y + 1, width, height, 15, 100);
    u8 windowId = AddWindow(&template);
    PutWindowTilemap(windowId);
    return windowId;
}

void ClearToTransparentAndRemoveWindow(u8 windowId)
{
    ClearStdWindowAndFrameToTransparent(windowId, TRUE);
    RemoveWindow(windowId);
}

static void DrawLinkServicesMultichoiceMenu(u8 multichoiceId)
{
    switch (multichoiceId)
    {
    case MULTI_WIRELESS_NO_BERRY:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, FONT_NORMAL, sWirelessOptionsNoBerryCrush[Menu_GetCursorPos()], 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        break;
    case MULTI_CABLE_CLUB_WITH_RECORD_MIX:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, FONT_NORMAL, sCableClubOptions_WithRecordMix[Menu_GetCursorPos()], 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        break;
    case MULTI_WIRELESS_NO_RECORD:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, FONT_NORMAL, sWirelessOptions_NoRecordMix[Menu_GetCursorPos()], 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        break;
    case MULTI_WIRELESS_ALL_SERVICES:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, FONT_NORMAL, sWirelessOptions_AllServices[Menu_GetCursorPos()], 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        break;
    case MULTI_WIRELESS_NO_RECORD_BERRY:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, FONT_NORMAL, sWirelessOptions_NoRecordMixBerryCrush[Menu_GetCursorPos()], 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        break;
    case MULTI_CABLE_CLUB_NO_RECORD_MIX:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, FONT_NORMAL, sCableClubOptions_NoRecordMix[Menu_GetCursorPos()], 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        break;
    }
}

bool16 ScriptMenu_CreateStartMenuForPokenavTutorial(void)
{
    if (FuncIsActiveTask(Task_HandleMultichoiceInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        CreateStartMenuForPokenavTutorial();
        return TRUE;
    }
}

static void CreateStartMenuForPokenavTutorial(void)
{
    u8 windowId = CreateWindowFromRect(21, 0, 7, 18);
    SetStandardWindowBorderStyle(windowId, FALSE);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionPokedex, 8, 9, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionPokemon, 8, 25, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionBag, 8, 41, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionPokenav, 8, 57, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gSaveBlock2Ptr->playerName, 8, 73, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionSave, 8, 89, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionOption, 8, 105, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_MenuOptionExit, 8, 121, TEXT_SKIP_DRAW, NULL);
    InitMenuNormal(windowId, FONT_NORMAL, 0, 9, 16, ARRAY_COUNT(MultichoiceList_ForcedStartMenu), 0);
    InitMultichoiceNoWrap(FALSE, ARRAY_COUNT(MultichoiceList_ForcedStartMenu), windowId, MULTI_FORCED_START_MENU);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

#define tWindowId       data[6]

static void InitMultichoiceNoWrap(bool8 ignoreBPress, u8 unusedCount, u8 windowId, u8 multichoiceId)
{
    u8 taskId;
    sProcessInputDelay = 2;
    taskId = CreateTask(Task_HandleMultichoiceInput, 80);
    gTasks[taskId].tIgnoreBPress = ignoreBPress;
    gTasks[taskId].tDoWrap = 0;
    gTasks[taskId].tWindowId = windowId;
    gTasks[taskId].tMultichoiceId = multichoiceId;
}

#undef tLeft
#undef tTop
#undef tRight
#undef tBottom
#undef tIgnoreBPress
#undef tDoWrap
#undef tWindowId
#undef tMultichoiceId

static int DisplayTextAndGetWidthInternal(const u8 *str)
{
    u8 temp[64];
    StringExpandPlaceholders(temp, str);
    return GetStringWidth(FONT_NORMAL, temp, 0);
}

int DisplayTextAndGetWidth(const u8 *str, int prevWidth)
{
    int width = DisplayTextAndGetWidthInternal(str);
    if (width < prevWidth)
    {
        width = prevWidth;
    }
    return width;
}

int ConvertPixelWidthToTileWidth(int width)
{
    return (((width + 9) / 8) + 1) > MAX_MULTICHOICE_WIDTH ? MAX_MULTICHOICE_WIDTH : (((width + 9) / 8) + 1);
}

int ScriptMenu_AdjustLeftCoordFromWidth(int left, int width)
{
    int adjustedLeft = left;

    if (left + width > MAX_MULTICHOICE_WIDTH)
    {
        if (MAX_MULTICHOICE_WIDTH - width < 0)
        {
            adjustedLeft = 0;
        }
        else
        {
            adjustedLeft = MAX_MULTICHOICE_WIDTH - width;
        }
    }

    return adjustedLeft;
}

// FRLG
#define FOSSIL_PIC_PAL_NUM  13

bool8 OpenMuseumFossilPic(void)
{
    // u8 spriteId;
    // u8 taskId;
    // if (QL_AvoidDisplay(QL_DestroyAbortedDisplay) == TRUE)
    //     return TRUE;
    // if (FindTaskIdByFunc(Task_WaitMuseumFossilPic) != TASK_NONE)
    //     return FALSE;
    // if (gSpecialVar_0x8004 == SPECIES_KABUTOPS)
    // {
    //     LoadSpriteSheets(sMuseumKabutopsSprSheets);
    //     LoadPalette(sMuseumKabutopsSprPalette, OBJ_PLTT_ID(FOSSIL_PIC_PAL_NUM), sizeof(sMuseumKabutopsSprPalette));
    // }
    // else if (gSpecialVar_0x8004 == SPECIES_AERODACTYL)
    // {
    //     LoadSpriteSheets(sMuseumAerodactylSprSheets);
    //     LoadPalette(sMuseumAerodactylSprPalette, OBJ_PLTT_ID(FOSSIL_PIC_PAL_NUM), sizeof(sMuseumAerodactylSprPalette));
    // }
    // else
    // {
    //     return FALSE;
    // }
    // spriteId = CreateSprite(&sMuseumFossilSprTemplate, gSpecialVar_0x8005 * 8 + 40, gSpecialVar_0x8006 * 8 + 40, 0);
    // gSprites[spriteId].oam.paletteNum = FOSSIL_PIC_PAL_NUM;
    // taskId = CreateTask(Task_WaitMuseumFossilPic, 80);
    // gTasks[taskId].tWindowId = CreateWindowFromRect(gSpecialVar_0x8005, gSpecialVar_0x8006, 8, 8);
    // gTasks[taskId].tState = 0;
    // gTasks[taskId].tSpriteId = spriteId;
    // SetStandardWindowBorderStyle(gTasks[taskId].tWindowId, TRUE);
    // ScheduleBgCopyTilemapToVram(0);
    return TRUE;
}

bool8 CloseMuseumFossilPic(void)
{
    // u8 taskId = FindTaskIdByFunc(Task_WaitMuseumFossilPic);
    // if (taskId == TASK_NONE)
    //     return FALSE;
    // gTasks[taskId].tState++;
    return TRUE;
}

static const u8 sText_Other[] = _("OTHER");

void DrawSeagallopDestinationMenu(void)
{
    // 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
    u8 destinationId;
    u8 top;
    u8 numItems;
    u8 cursorWidth;
    u8 windowId;
    u8 i;
    gSpecialVar_Result = 0xFF;

    if (gSpecialVar_0x8005 == 1)
    {
        if (gSpecialVar_0x8004 < SEAGALLOP_FIVE_ISLAND)
            destinationId = SEAGALLOP_FIVE_ISLAND;
        else
            destinationId = SEAGALLOP_FOUR_ISLAND;
        numItems = 5;
        top = 2;
    }
    else
    {
        destinationId = SEAGALLOP_VERMILION_CITY;
        numItems = 6;
        top = 0;
    }
    cursorWidth = GetMenuCursorDimensionByFont(FONT_NORMAL, 0);
    windowId = CreateWindowFromRect(17, top, 11, numItems * 2);
    SetStandardWindowBorderStyle(windowId, FALSE);

    // -2 excludes "Other" and "Exit", appended after the loop
    for (i = 0; i < numItems - 2; i++)
    {
        if (destinationId != gSpecialVar_0x8004)
            AddTextPrinterParameterized(windowId, FONT_NORMAL, sSeagallopDestStrings[destinationId], cursorWidth, i * 16 + 2, TEXT_SKIP_DRAW, NULL);
        else
            i--;
        destinationId++;

        // Wrap around
        if (destinationId == SEAGALLOP_SEVEN_ISLAND + 1)
            destinationId = SEAGALLOP_VERMILION_CITY;
    }
    AddTextPrinterParameterized(windowId, FONT_NORMAL, sText_Other, cursorWidth, i * 16 + 2, TEXT_SKIP_DRAW, NULL);
    i++;
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_Exit, cursorWidth, i * 16 + 2, TEXT_SKIP_DRAW, NULL);
    InitMenuNormal(windowId, FONT_NORMAL, 0, 2, 16, numItems, 0);
    InitMultichoiceCheckWrap(FALSE, numItems, windowId, MULTI_NONE);
    ScheduleBgCopyTilemapToVram(0);
}

u16 GetSelectedSeagallopDestination(void)
{
    // 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
    if (gSpecialVar_Result == MULTI_B_PRESSED)
        return MULTI_B_PRESSED;
    if (gSpecialVar_0x8005 == 1)
    {
        if (gSpecialVar_Result == 3)
        {
            return SEAGALLOP_MORE;
        }
        else if (gSpecialVar_Result == 4)
        {
            return MULTI_B_PRESSED;
        }
        else if (gSpecialVar_Result == 0)
        {
            if (gSpecialVar_0x8004 > SEAGALLOP_FOUR_ISLAND)
                return SEAGALLOP_FOUR_ISLAND;
            else
                return SEAGALLOP_FIVE_ISLAND;
        }
        else if (gSpecialVar_Result == 1)
        {
            if (gSpecialVar_0x8004 > SEAGALLOP_FIVE_ISLAND)
                return SEAGALLOP_FIVE_ISLAND;
            else
                return SEAGALLOP_SIX_ISLAND;
        }
        else if (gSpecialVar_Result == 2)
        {
            if (gSpecialVar_0x8004 > SEAGALLOP_SIX_ISLAND)
                return SEAGALLOP_SIX_ISLAND;
            else
                return SEAGALLOP_SEVEN_ISLAND;
        }
    }
    else
    {
        if (gSpecialVar_Result == 4)
            return SEAGALLOP_MORE;
        else if (gSpecialVar_Result == 5)
            return MULTI_B_PRESSED;
        else if (gSpecialVar_Result >= gSpecialVar_0x8004)
            return gSpecialVar_Result + 1;
        else
            return gSpecialVar_Result;
    }
    return SEAGALLOP_VERMILION_CITY;
}
