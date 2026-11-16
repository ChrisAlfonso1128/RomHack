Route23_UnusedHouse_MapScripts::
    .byte 0

HmTestLab_EventScript_EnterCut::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_CUT, 3, 5
    waitstate
    end
