HmLab_Cut_MapScripts::
    .byte 0

HmLab_Cut_EventScript_Check::
    lockall
    checkflag FLAG_BADGE02_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_Cut_EventScript_Locked
    msgbox HmLab_Cut_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Cut_EventScript_Locked::
    msgbox HmLab_Cut_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Cut_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Cut_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_FLASH, 3, 5
    waitstate
    end

HmLab_Cut_Text_Unlocked::
    .string "CUT TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_Cut_Text_Locked::
    .string "CUT TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
