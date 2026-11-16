HmLab_Fly_MapScripts::
    .byte 0

HmLab_Fly_EventScript_Check::
    lockall
    checkflag FLAG_BADGE03_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_Fly_EventScript_Locked
    msgbox HmLab_Fly_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Fly_EventScript_Locked::
    msgbox HmLab_Fly_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Fly_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Fly_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_STRENGTH, 3, 5
    waitstate
    end

HmLab_Fly_Text_Unlocked::
    .string "FLY TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_Fly_Text_Locked::
    .string "FLY TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
