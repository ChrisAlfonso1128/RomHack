Route23_UnusedHouse_MapScripts::
    .byte 0

HmTestLab_EventScript_CutStation::
    lockall
    checkflag FLAG_BADGE02_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_CutLocked
    msgbox HmTestLab_Text_CutReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_CutLocked::
    msgbox HmTestLab_Text_CutLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_CutReady::
    .string "CUT - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_CutLocked::
    .string "CUT - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

HmTestLab_EventScript_FlashStation::
    lockall
    checkflag FLAG_BADGE01_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_FlashLocked
    msgbox HmTestLab_Text_FlashReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_FlashLocked::
    msgbox HmTestLab_Text_FlashLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_FlashReady::
    .string "FLASH - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_FlashLocked::
    .string "FLASH - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

HmTestLab_EventScript_FlyStation::
    lockall
    checkflag FLAG_BADGE03_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_FlyLocked
    msgbox HmTestLab_Text_FlyReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_FlyLocked::
    msgbox HmTestLab_Text_FlyLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_FlyReady::
    .string "FLY - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_FlyLocked::
    .string "FLY - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

HmTestLab_EventScript_StrengthStation::
    lockall
    checkflag FLAG_BADGE04_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_StrengthLocked
    msgbox HmTestLab_Text_StrengthReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_StrengthLocked::
    msgbox HmTestLab_Text_StrengthLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_StrengthReady::
    .string "STRENGTH - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_StrengthLocked::
    .string "STRENGTH - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

HmTestLab_EventScript_SurfStation::
    lockall
    checkflag FLAG_BADGE05_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_SurfLocked
    msgbox HmTestLab_Text_SurfReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_SurfLocked::
    msgbox HmTestLab_Text_SurfLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_SurfReady::
    .string "SURF - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_SurfLocked::
    .string "SURF - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

HmTestLab_EventScript_RockSmashStation::
    lockall
    checkflag FLAG_BADGE06_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_RockSmashLocked
    msgbox HmTestLab_Text_RockSmashReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_RockSmashLocked::
    msgbox HmTestLab_Text_RockSmashLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_RockSmashReady::
    .string "ROCKSMASH - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_RockSmashLocked::
    .string "ROCKSMASH - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

HmTestLab_EventScript_WaterfallStation::
    lockall
    checkflag FLAG_BADGE07_GET
    goto_if_eq VAR_RESULT, FALSE, HmTestLab_EventScript_WaterfallLocked
    msgbox HmTestLab_Text_WaterfallReady, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_EventScript_WaterfallLocked::
    msgbox HmTestLab_Text_WaterfallLocked, MSGBOX_DEFAULT
    releaseall
    end

HmTestLab_Text_WaterfallReady::
    .string "WATERFALL - BADGE READY\p"
    .string "Badge present. Test the field move\n"
    .string "using your current game mode.$"

HmTestLab_Text_WaterfallLocked::
    .string "WATERFALL - BADGE MISSING\p"
    .string "Use the badge desk to unlock\n"
    .string "this field move requirement.$"

