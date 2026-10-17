BattleTestLab_MapScripts::
    .byte 0

BattleTestLab_EventScript_ExitGuide::
    lock
    faceplayer
    msgbox BattleTestLab_Text_ExitGuide
    closemessage
    warp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 11, 8
    waitstate
    release
    end

BattleTestLab_Text_ExitGuide::
    .string "BATTLE LAB: Returning to OAK's LAB.$"

BattleTestLab_EventScript_Brock::
    cleartrainerflag TRAINER_LEADER_BROCK
    trainerbattle_single TRAINER_LEADER_BROCK, BattleTestLab_Text_BrockIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_BrockIntro::
    .string "BROCK: Ready for a test battle?$"

BattleTestLab_EventScript_Misty::
    cleartrainerflag TRAINER_LEADER_MISTY
    trainerbattle_single TRAINER_LEADER_MISTY, BattleTestLab_Text_MistyIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_MistyIntro::
    .string "MISTY: Ready for a test battle?$"

BattleTestLab_EventScript_Surge::
    cleartrainerflag TRAINER_LEADER_LT_SURGE
    trainerbattle_single TRAINER_LEADER_LT_SURGE, BattleTestLab_Text_SurgeIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_SurgeIntro::
    .string "SURGE: Ready for a test battle?$"

BattleTestLab_EventScript_Erika::
    cleartrainerflag TRAINER_LEADER_ERIKA
    trainerbattle_single TRAINER_LEADER_ERIKA, BattleTestLab_Text_ErikaIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_ErikaIntro::
    .string "ERIKA: Ready for a test battle?$"

BattleTestLab_EventScript_Koga::
    cleartrainerflag TRAINER_LEADER_KOGA
    trainerbattle_single TRAINER_LEADER_KOGA, BattleTestLab_Text_KogaIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_KogaIntro::
    .string "KOGA: Ready for a test battle?$"

BattleTestLab_EventScript_Sabrina::
    cleartrainerflag TRAINER_LEADER_SABRINA
    trainerbattle_single TRAINER_LEADER_SABRINA, BattleTestLab_Text_SabrinaIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_SabrinaIntro::
    .string "SABRINA: Ready for a test battle?$"

BattleTestLab_EventScript_Blaine::
    cleartrainerflag TRAINER_LEADER_BLAINE
    trainerbattle_single TRAINER_LEADER_BLAINE, BattleTestLab_Text_BlaineIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_BlaineIntro::
    .string "BLAINE: Ready for a test battle?$"

BattleTestLab_EventScript_Giovanni::
    cleartrainerflag TRAINER_LEADER_GIOVANNI
    trainerbattle_single TRAINER_LEADER_GIOVANNI, BattleTestLab_Text_GiovanniIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_GiovanniIntro::
    .string "GIOVANNI: Ready for a test battle?$"

BattleTestLab_EventScript_EliteOne::
    cleartrainerflag TRAINER_ELITE_FOUR_LORELEI
    trainerbattle_single TRAINER_ELITE_FOUR_LORELEI, BattleTestLab_Text_EliteOneIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_EliteOneIntro::
    .string "E4 ONE: Ready for a test battle?$"

BattleTestLab_EventScript_EliteTwo::
    cleartrainerflag TRAINER_ELITE_FOUR_BRUNO
    trainerbattle_single TRAINER_ELITE_FOUR_BRUNO, BattleTestLab_Text_EliteTwoIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_EliteTwoIntro::
    .string "E4 TWO: Ready for a test battle?$"

BattleTestLab_EventScript_EliteThree::
    cleartrainerflag TRAINER_ELITE_FOUR_AGATHA
    trainerbattle_single TRAINER_ELITE_FOUR_AGATHA, BattleTestLab_Text_EliteThreeIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_EliteThreeIntro::
    .string "E4 THREE: Ready for a test battle?$"

BattleTestLab_EventScript_EliteFour::
    cleartrainerflag TRAINER_ELITE_FOUR_LANCE
    trainerbattle_single TRAINER_ELITE_FOUR_LANCE, BattleTestLab_Text_EliteFourIntro, BattleTestLab_Text_BattleDone
    msgbox BattleTestLab_Text_Retry, MSGBOX_DEFAULT
    end

BattleTestLab_Text_EliteFourIntro::
    .string "E4 FOUR: Ready for a test battle?$"

BattleTestLab_Text_BattleDone::
    .string "Good test battle!$"

BattleTestLab_Text_Retry::
    .string "Talk to me again for a rematch.$"

BattleTestLab_EventScript_Heal::
    lock
    faceplayer
    call Common_EventScript_OutOfCenterPartyHeal
    msgbox BattleTestLab_Text_Healed
    release
    end

BattleTestLab_Text_Healed::
    .string "Your party is fully healed.$"

BattleTestLab_EventScript_Supplies::
    lock
    faceplayer
    giveitem ITEM_RARE_CANDY, 99
    giveitem ITEM_MAX_REVIVE, 20
    giveitem ITEM_FULL_RESTORE, 20
    msgbox BattleTestLab_Text_Supplies
    release
    end

BattleTestLab_Text_Supplies::
    .string "Test supplies issued. Keep your\\n"
    .string "party ready for repeat battles.$"

BattleTestLab_EventScript_TestTeam::
    lock
    faceplayer
    msgbox BattleTestLab_Text_TestTeamIntro
    givemon SPECIES_LUCARIO, 55
    givemon SPECIES_GYARADOS, 55
    givemon SPECIES_GARDEVOIR, 55
    givemon SPECIES_EXCADRILL, 55
    givemon SPECIES_VENUSAUR, 55
    givemon SPECIES_ROTOM_WASH, 55
    msgbox BattleTestLab_Text_TestTeamGiven
    release
    end

BattleTestLab_Text_TestTeamIntro::
    .string "Here is a balanced LV55 test team.\\p"
    .string "Make room first, or use your PC.$"

BattleTestLab_Text_TestTeamGiven::
    .string "Team issued! Test with care.$"
