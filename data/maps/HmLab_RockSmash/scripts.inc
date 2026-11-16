HmLab_RockSmash_MapScripts::
    .byte 0

HmLab_RockSmash_EventScript_Check::
    lockall
    checkflag FLAG_BADGE06_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_RockSmash_EventScript_Locked
    msgbox HmLab_RockSmash_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_RockSmash_EventScript_Locked::
    msgbox HmLab_RockSmash_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_RockSmash_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_RockSmash_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_WATERFALL, 3, 5
    waitstate
    end

HmLab_RockSmash_Text_Unlocked::
    .string "ROCKSMASH TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_RockSmash_Text_Locked::
    .string "ROCKSMASH TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
