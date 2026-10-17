BattleTestLab_MapScripts::
    .byte 0

BattleTestLab_EventScript_ExitGuide::
    lock
    faceplayer
    msgbox BattleTestLab_Text_ExitGuide
    closemessage
    warp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 11, 8
    waitstate
    release
    end

BattleTestLab_Text_ExitGuide::
    .string "This is the BATTLE TEST LAB.\p"
    .string "Trainer stations are coming soon.\p"
    .string "Returning to OAK's LAB now.$"
