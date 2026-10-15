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
	msgbox PalletTown_PlayersHouse_2F_Text_PlayedWithNES, MSGBOX_SIGN
	end

PalletTown_PlayersHouse_2F_EventScript_Sign::
	lockall
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
	.string "ADVENTURE SETTINGS\\p"
	.string "Choose which POKeMON generations\\n"
	.string "will appear on your journey.\\p"
	.string "Some choices become permanent\\n"
	.string "when you leave your room.\\p"
	.string "Other options, like EXP sharing,\\n"
	.string "can be changed later.$"

PalletTown_PlayersHouse_2F_EventScript_AdventureSettings::
    msgbox PalletTown_PlayersHouse_2F_Text_AdventureSettings, MSGBOX_DEFAULT
PalletTown_PlayersHouse_2F_EventScript_AdventureMenu::
    multichoice 1, 1, MULTI_ADVENTURE_SETTINGS, FALSE
    switch VAR_RESULT
    case 0, PalletTown_PlayersHouse_2F_EventScript_GenFirst
    case 1, PalletTown_PlayersHouse_2F_EventScript_FutureOptions
    goto PalletTown_PlayersHouse_2F_EventScript_SettingsExit

PalletTown_PlayersHouse_2F_EventScript_FutureOptions::
    msgbox PalletTown_PlayersHouse_2F_Text_FutureOptions, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_GenFirst::
    msgbox PalletTown_PlayersHouse_2F_Text_GenPrompt, MSGBOX_DEFAULT
PalletTown_PlayersHouse_2F_EventScript_GenFirstMenu::
    multichoice 1, 1, MULTI_ADVENTURE_GEN_FIRST, FALSE
    switch VAR_RESULT
    case 0, PalletTown_PlayersHouse_2F_EventScript_Gen1
    case 1, PalletTown_PlayersHouse_2F_EventScript_Gen2
    case 2, PalletTown_PlayersHouse_2F_EventScript_Gen3
    case 3, PalletTown_PlayersHouse_2F_EventScript_Gen4
    case 4, PalletTown_PlayersHouse_2F_EventScript_Gen5
    case 5, PalletTown_PlayersHouse_2F_EventScript_GenSecond
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_GenSecond::
    multichoice 1, 1, MULTI_ADVENTURE_GEN_SECOND, FALSE
    switch VAR_RESULT
    case 0, PalletTown_PlayersHouse_2F_EventScript_Gen6
    case 1, PalletTown_PlayersHouse_2F_EventScript_Gen7
    case 2, PalletTown_PlayersHouse_2F_EventScript_Gen8
    case 3, PalletTown_PlayersHouse_2F_EventScript_Gen9
    case 4, PalletTown_PlayersHouse_2F_EventScript_GenAll
    goto PalletTown_PlayersHouse_2F_EventScript_GenFirstMenu

PalletTown_PlayersHouse_2F_EventScript_Gen1::
    setvar VAR_0x8005, 1
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen2::
    setvar VAR_0x8005, 2
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen3::
    setvar VAR_0x8005, 3
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen4::
    setvar VAR_0x8005, 4
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen5::
    setvar VAR_0x8005, 5
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen6::
    setvar VAR_0x8005, 6
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen7::
    setvar VAR_0x8005, 7
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen8::
    setvar VAR_0x8005, 8
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_Gen9::
    setvar VAR_0x8005, 9
    goto PalletTown_PlayersHouse_2F_EventScript_GenMode

PalletTown_PlayersHouse_2F_EventScript_GenAll::
    setvar VAR_GENERATION_SELECTION, 0
    msgbox PalletTown_PlayersHouse_2F_Text_GenSaved, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_GenMode::
    msgbox PalletTown_PlayersHouse_2F_Text_ModePrompt, MSGBOX_DEFAULT
    multichoice 1, 1, MULTI_ADVENTURE_GEN_MODE, FALSE
    switch VAR_RESULT
    case 0, PalletTown_PlayersHouse_2F_EventScript_GenOnly
    case 1, PalletTown_PlayersHouse_2F_EventScript_GenUnlocked
    goto PalletTown_PlayersHouse_2F_EventScript_GenFirstMenu

PalletTown_PlayersHouse_2F_EventScript_GenOnly::
    copyvar VAR_GENERATION_SELECTION, VAR_0x8005
    msgbox PalletTown_PlayersHouse_2F_Text_GenSaved, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_GenUnlocked::
    addvar VAR_0x8005, 16
    copyvar VAR_GENERATION_SELECTION, VAR_0x8005
    msgbox PalletTown_PlayersHouse_2F_Text_GenSaved, MSGBOX_DEFAULT
    goto PalletTown_PlayersHouse_2F_EventScript_AdventureMenu

PalletTown_PlayersHouse_2F_EventScript_SettingsExit::
    releaseall
    end

PalletTown_PlayersHouse_2F_Text_GenPrompt::
    .string "Select which generations of\\n"
    .string "POKeMON can appear.$"

PalletTown_PlayersHouse_2F_Text_ModePrompt::
    .string "ONLY: Just the selected generation.\\p"
    .string "UNLOCKED: That generation and\\n"
    .string "all earlier generations.$"

PalletTown_PlayersHouse_2F_Text_GenSaved::
    .string "Generation setting updated!$"

PalletTown_PlayersHouse_2F_Text_FutureOptions::
    .string "More adventure settings will\\n"
    .string "be added here later.$"
