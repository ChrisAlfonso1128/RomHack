PalletTown_PlayersHouse_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_PlayersHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PalletTown_PlayersHouse_2F_OnWarp
	.byte 0

PalletTown_PlayersHouse_2F_OnTransition::
	call_if_eq VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_EventScript_SetRespawn
	end

PalletTown_PlayersHouse_2F_EventScript_SetRespawn::
	setrespawn HEAL_LOCATION_PALLET_TOWN
	return

PalletTown_PlayersHouse_2F_OnWarp::
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_FirstWarpIn
	.2byte 0

PalletTown_PlayersHouse_2F_FirstWarpIn::
	turnobject LOCALID_PLAYER, DIR_NORTH
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 1
	end

PalletTown_PlayersHouse_2F_EventScript_NES::
    lockall
    msgbox PalletTown_PlayersHouse_2F_Text_HmLabPrompt, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, PalletTown_PlayersHouse_2F_EventScript_HmLabWarp
    releaseall
    end

PalletTown_PlayersHouse_2F_EventScript_Sign::
	lockall
	goto_if_eq VAR_ADVENTURE_SETTINGS_CONFIRMED, 1, PalletTown_PlayersHouse_2F_EventScript_SettingsExit
	goto PalletTown_PlayersHouse_2F_EventScript_AdventureSettings

PalletTown_PlayersHouse_2F_Text_PlayedWithNES::
	.string "{PLAYER} played with the NES.\p"
	.string "…Okay!\n"
	.string "It's time to go!$"

PalletTown_PlayersHouse_2F_Text_PressLRForHelp::
	.string "It's a posted notice…\p"
	.string "If you're confused, ask for HELP!\n"
	.string "Press the L or R Button!$"


PalletTown_PlayersHouse_2F_Text_AdventureSettings::
	.string "ADVENTURE SETTINGS\p"
	.string "Choose which POKeMON generations\n"
	.string "will appear on your journey.\p"
	.string "Some choices become permanent\n"
	.string "when you leave your room.\p"
	.string "Other options, like EXP sharing,\n"
	.string "can be changed later.$"

PalletTown_PlayersHouse_2F_EventScript_AdventureSettings::
    @ Open the actual selectable menu immediately; cursor help is drawn below it.
PalletTown_PlayersHouse_2F_EventScript_AdventureMenu::
    closemessage
    multichoice 1, 1, MULTI_ADVENTURE_SETTINGS, FALSE
    switch VAR_RESULT
    case 0, PalletTown_PlayersHouse_2F_EventScript_GenFirst
    case 1, PalletTown_PlayersHouse_2F_EventScript_ExpShare
    case 2, PalletTown_PlayersHouse_2F_EventScript_BattleMechanics
    goto PalletTown_PlayersHouse_2F_EventScript_SettingsExit

PalletTown_PlayersHouse_2F_EventScript_BattleMechanics::
    multichoice 1, 1, MULTI_ADVENTURE_BATTLE_MECHANICS, FALSE
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_ExpShare::
    multichoice 1, 1, MULTI_ADVENTURE_EXP_SHARE, FALSE
    switch VAR_RESULT
    case 0, PalletTown_PlayersHouse_2F_EventScript_ExpModern
    case 1, PalletTown_PlayersHouse_2F_EventScript_ExpClassic
    case 2, PalletTown_PlayersHouse_2F_EventScript_ExpOff
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_ExpModern::
    setvar VAR_ADVENTURE_EXP_SHARE_MODE, 0
    goto PalletTown_PlayersHouse_2F_EventScript_ExpSaved

PalletTown_PlayersHouse_2F_EventScript_ExpClassic::
    setvar VAR_ADVENTURE_EXP_SHARE_MODE, 1
    goto PalletTown_PlayersHouse_2F_EventScript_ExpSaved

PalletTown_PlayersHouse_2F_EventScript_ExpOff::
    setvar VAR_ADVENTURE_EXP_SHARE_MODE, 2

PalletTown_PlayersHouse_2F_EventScript_ExpSaved::
    setvar VAR_ADVENTURE_EXP_CHOSEN, 1
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_Text_ExpSharePrompt::
    .string "Choose how experience is shared.\n"
    .string "This setting can change anytime.$"

PalletTown_PlayersHouse_2F_Text_ExpSaved::
    .string "EXP Share preference saved.$"

PalletTown_PlayersHouse_2F_EventScript_FutureOptions::
    msgbox PalletTown_PlayersHouse_2F_Text_FutureOptions, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

@ Bag-style checklist. Each generation is a separate checkbox; no forced ONLY/UNLOCKED choice.
PalletTown_PlayersHouse_2F_EventScript_GenFirst::
    goto_if_eq VAR_ADVENTURE_SETTINGS_CONFIRMED, 1, PalletTown_PlayersHouse_2F_EventScript_GenLocked
    multichoice 1, 1, MULTI_ADVENTURE_GEN_FIRST, FALSE
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_SettingsExit::
    releaseall
    end

PalletTown_PlayersHouse_2F_Text_GenPrompt::
    .string "Select which generations of\n"
    .string "POKeMON can appear.$"

PalletTown_PlayersHouse_2F_Text_ModePrompt::
    .string "ONLY: Just the selected generation.\p"
    .string "UNLOCKED: That generation and\n"
    .string "all earlier generations.$"

PalletTown_PlayersHouse_2F_Text_GenSaved::
    .string "Generation setting updated!$"

PalletTown_PlayersHouse_2F_Text_FutureOptions::
    .string "More adventure settings will\n"
    .string "be added here later.$"

PalletTown_PlayersHouse_2F_EventScript_GenLocked::
    msgbox PalletTown_PlayersHouse_2F_Text_GenLocked, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_ConfirmSettings::
    lockall
    @ If the player skipped either required menu, offer defaults at the stairs.
    goto_if_eq VAR_ADVENTURE_GEN_CHOSEN, 0, PalletTown_PlayersHouse_2F_EventScript_OfferDefaults
    goto_if_eq VAR_ADVENTURE_EXP_CHOSEN, 0, PalletTown_PlayersHouse_2F_EventScript_OfferDefaults
PalletTown_PlayersHouse_2F_EventScript_FinalizeSettings::
    @ Settings become final only on the stairs.
    setvar VAR_ADVENTURE_SETTINGS_CONFIRMED, 1
    @ Complete the stair transition here instead of making the player
    @ re-enter a coordinate trigger on the warp tile.
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_PALLET_TOWN_PLAYERS_HOUSE_1F, 2, 0, 0
    waitstate
    end

PalletTown_PlayersHouse_2F_EventScript_OfferDefaults::
    applymovement LOCALID_PLAYER, PalletTown_PlayersHouse_2F_Movement_StepAwayFromStairs
    waitmovement 0
    msgbox PalletTown_PlayersHouse_2F_Text_DefaultSettingsPrompt, MSGBOX_DEFAULT
    yesnobox 20, 8
    goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_SettingsExit
    @ Unselected generations use the existing all-generations default (0).
    @ Unselected EXP Share uses the existing Modern default (0).
    setvar VAR_ADVENTURE_GEN_CHOSEN, 1
    setvar VAR_ADVENTURE_EXP_CHOSEN, 1
    goto PalletTown_PlayersHouse_2F_EventScript_FinalizeSettings

PalletTown_PlayersHouse_2F_Text_DefaultSettingsPrompt::
    .string "Continue with default settings?\p"
    .string "All generations and Modern EXP\n"
    .string "Share will be used if not chosen.$"

PalletTown_PlayersHouse_2F_EventScript_ReviewSettings::
    applymovement LOCALID_PLAYER, PalletTown_PlayersHouse_2F_Movement_StepAwayFromStairs
    waitmovement 0
    msgbox PalletTown_PlayersHouse_2F_Text_ReviewSettings, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu
    end

PalletTown_PlayersHouse_2F_Text_ConfirmSettings::
    .string "Your checked generations\n"
    .string "will be permanent after you leave.\p"
    .string "Confirm your adventure settings?$"

PalletTown_PlayersHouse_2F_Text_SettingsLocked::
    .string "Adventure settings confirmed!\n"
    .string "Your generation is now locked.$"

PalletTown_PlayersHouse_2F_Text_ReviewSettings::
    .string "Review your settings using the\n"
    .string "notice in your room.$"

PalletTown_PlayersHouse_2F_Text_GenLocked::
    .string "Your generation is locked.\n"
    .string "Other options may still change.$"

PalletTown_PlayersHouse_2F_Movement_StepAwayFromStairs:
    walk_down
    step_end

@ HM test access point (prototype). Does not grant badges or bypass story gates.
PalletTown_PlayersHouse_2F_EventScript_HmTestDesk::
    lockall
    msgbox PalletTown_PlayersHouse_2F_Text_HmTestDesk, MSGBOX_DEFAULT
    releaseall
    end

PalletTown_PlayersHouse_2F_Text_HmTestDesk::
    .string "HM TEST LAB\p"
    .string "Field move testing is being built.\n"
    .string "No badges have been changed.$"

PalletTown_PlayersHouse_2F_Text_HmLabPrompt::
    .string "HM TEST LAB\p"
    .string "Teleport to the testing room?$"

PalletTown_PlayersHouse_2F_EventScript_HmLabWarp::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmTestLab_EventScript_Return::
    lockall
    msgbox HmTestLab_Text_Return, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, HmTestLab_EventScript_Exit
    releaseall
    end

HmTestLab_EventScript_Exit::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_PALLET_TOWN_PLAYERS_HOUSE_2F, 6, 6
    waitstate
    end

HmTestLab_Text_Return::
    .string "HM TEST LAB\p"
    .string "Return to your bedroom?$"

HmTestLab_EventScript_Station::
    msgbox HmTestLab_Text_Station, MSGBOX_SIGN
    end

HmTestLab_Text_Station::
    .string "HM FIELD TEST\p"
    .string "More challenge stations coming soon.$"


@ Lab-only test controls. Badge state is changed only after explicit confirmation.
HmTestLab_EventScript_Badges::
    lockall
    msgbox HmTestLab_Text_Badges, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, HmTestLab_EventScript_GrantBadges
    msgbox HmTestLab_Text_ResetBadges, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, HmTestLab_EventScript_ClearBadges
    releaseall
    end

HmTestLab_EventScript_GrantBadges::
    setflag FLAG_BADGE01_GET
    setflag FLAG_BADGE02_GET
    setflag FLAG_BADGE03_GET
    setflag FLAG_BADGE04_GET
    setflag FLAG_BADGE05_GET
    setflag FLAG_BADGE06_GET
    setflag FLAG_BADGE07_GET
    setflag FLAG_BADGE08_GET
    msgbox HmTestLab_Text_Granted, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_ClearBadges::
    clearflag FLAG_BADGE01_GET
    clearflag FLAG_BADGE02_GET
    clearflag FLAG_BADGE03_GET
    clearflag FLAG_BADGE04_GET
    clearflag FLAG_BADGE05_GET
    clearflag FLAG_BADGE06_GET
    clearflag FLAG_BADGE07_GET
    clearflag FLAG_BADGE08_GET
    msgbox HmTestLab_Text_Cleared, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_Items::
    lockall
    msgbox HmTestLab_Text_Items, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, HmTestLab_EventScript_GrantItems
    releaseall
    end

HmTestLab_EventScript_GrantItems::
    additem ITEM_HM01, 1
    additem ITEM_HM02, 1
    additem ITEM_HM03, 1
    additem ITEM_HM04, 1
    additem ITEM_HM05, 1
    additem ITEM_HM06, 1
    additem ITEM_HM07, 1
    additem ITEM_HM08, 1
    msgbox HmTestLab_Text_ItemsGranted, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_Badges::
    .string "Grant all eight badges?\n"
    .string "Use a disposable test save.$"

HmTestLab_Text_ResetBadges::
    .string "Clear all eight badges?\n"
    .string "This changes your save.$"

HmTestLab_Text_Granted::
    .string "All badges enabled.$"

HmTestLab_Text_Cleared::
    .string "All badges cleared.$"

HmTestLab_Text_Items::
    .string "Receive all eight HM items?\n"
    .string "Bag space is required.$"

HmTestLab_Text_ItemsGranted::
    .string "HM items added where possible.$"
