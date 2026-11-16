HmLab_Strength_MapScripts::
    .byte 0

HmLab_Strength_EventScript_Check::
    lockall
    checkflag FLAG_BADGE04_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_Strength_EventScript_Locked
    msgbox HmLab_Strength_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Strength_EventScript_Locked::
    msgbox HmLab_Strength_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Strength_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Strength_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_SURF, 3, 5
    waitstate
    end

HmLab_Strength_Text_Unlocked::
    .string "STRENGTH TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_Strength_Text_Locked::
    .string "STRENGTH TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
