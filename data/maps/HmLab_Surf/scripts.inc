HmLab_Surf_MapScripts::
    .byte 0

HmLab_Surf_EventScript_Check::
    lockall
    checkflag FLAG_BADGE05_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_Surf_EventScript_Locked
    msgbox HmLab_Surf_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Surf_EventScript_Locked::
    msgbox HmLab_Surf_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Surf_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Surf_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_ROCKSMASH, 3, 5
    waitstate
    end

HmLab_Surf_Text_Unlocked::
    .string "SURF TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_Surf_Text_Locked::
    .string "SURF TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
