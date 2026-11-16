Route23_UnusedHouse_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, HmTestLab_EventScript_Welcome
    .byte 0

HmTestLab_EventScript_Welcome::
    msgbox HmTestLab_Text_Welcome, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, HmTestLab_EventScript_EnterCut
    end

HmTestLab_Text_Welcome::
    .string "HM TEST LAB\\p"
    .string "Enter the seven-room HM course?\n"
    .string "Choose YES to begin with CUT.$"

HmTestLab_EventScript_EnterCut::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_CUT, 3, 5
    waitstate
    end
