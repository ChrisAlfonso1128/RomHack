HmLab_Flash_MapScripts::
    .byte 0

HmLab_Flash_EventScript_Check::
    lockall
    checkflag FLAG_BADGE01_GET
    goto_if_eq VAR_RESULT, FALSE, HmLab_Flash_EventScript_Locked
    msgbox HmLab_Flash_Text_Unlocked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Flash_EventScript_Locked::
    msgbox HmLab_Flash_Text_Locked, MSGBOX_DEFAULT
    releaseall
    end

HmLab_Flash_EventScript_Return::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_ROUTE23_UNUSED_HOUSE, 3, 5
    waitstate
    end

HmLab_Flash_EventScript_Next::
    closemessage
    fadescreen FADE_TO_BLACK
    warp MAP_HM_LAB_FLY, 3, 5
    waitstate
    end

HmLab_Flash_Text_Unlocked::
    .string "FLASH TEST\p"
    .string "Required badge is present.\n"
    .string "Try the field move in this room.$"

HmLab_Flash_Text_Locked::
    .string "FLASH TEST\p"
    .string "Required badge is missing.\n"
    .string "Use the lab badge control.$"
