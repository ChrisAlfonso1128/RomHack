HmLab_Waterfall_MapScripts::
    .byte 0

HmLab_Waterfall_EventScript_Check::
    lockall
    checkflag FLAG_BADGE07_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_Waterfall_EventScript_Locked
    msgbox HmLab_Waterfall_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Waterfall_EventScript_Locked::
    msgbox HmLab_Waterfall_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Waterfall_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Waterfall_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Waterfall_Text_Unlocked::
    .string "WATERFALL TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_Waterfall_Text_Locked::
    .string "WATERFALL TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
