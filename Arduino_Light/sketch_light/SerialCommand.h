#ifndef SERIALCOMMAND_HPP
#define SERIALCOMMAND_HPP

enum ePageID
{
    eMAIN = 0,
    eLight = 1,
    ePressair = 2,
    eKlimate = 3,
    eElectNetWork = 4,
    eDoor = 5 ,
    eSettings = 6
};

//global cmd

#define SHOW_MAIN_MENU           "MMMS1"        //SHOW MAIN MENU

#define CMD_SETBRT               "M$BRT000000" //SET BRIG 0-1024
#define CMD_SLEEP                "MSLP1"       //GO TO SLEEP

//
//MAINPAGECMD
//
#define SHOW_LIGHT_PAGE          "LIG01" //SHOW LIGHT PAGE, ASK FOR INVALIDATE
#define SHOW_PRESSAIR_PAGE       "PRES1" //SHOW PRESSAIR PAGE, ASK FOR INVALIDATE
#define SHOW_KLIMATE_PAGE        "KLIM1" //SHOW KLIMATE PAGE, ASK FOR INVALIDATE
#define SHOW_ELECT_NET_PAGE      "ENET1" //SHOW ELECTRIC PAGE, ASK FOR INVALIDATE
#define SHOW_DOOR_PAGE           "DOOR1" //SHOW DOOR PAGE, ASK FOR INVALIDATE
#define SHOW_SETTINGS_PAGE       "SETT1" //SHOW SETTINGS PAGE, ASK FOR INVALIDATE

//LIGHT PAGE
#define CMD_INVALIDATE_A "L|A"//"L|A0000000000000000" //from GATE SENS LAMP TO LAST SEND ROOM LAMP
#define CMD_INVALIDATE_B "L|B"//"L|B00000000"         //WALL LAMP TO BTN

#define BTN_AUTO_ALL             "LATO1"  // BTN AUTO ALL
#define BTN_AUTO_CEIL            "LACE1"  // BTN AUTO CEIL
#define BTN_AUTO_SING            "LASI1"  // BTN AUTO SINGLE

#define AUTO_BULB_LIGHT_1        "LbsG1" //  BULB 2
#define AUTO_BULB_LIGHT_1_SENS   "LsaG1" //  BULB 2 SENS

#define AUTO_BULB_LIGHT_2        "LbsD1" //  BULB 2
#define AUTO_BULB_LIGHT_2_SENS   "LsaD1" //  BULB 2 SENS

#define AUTO_BULB_LIGHT_3        "Lbs11" //  BULB 3
#define AUTO_BULB_LIGHT_3_SENS   "Lsa11" //  BULB 3 SENS

#define AUTO_BULB_LIGHT_4        "Lbs21" //  BULB 4
#define AUTO_BULB_LIGHT_4_SENS   "Lsa21" //  BULB 4 SENS

#define AUTO_BULB_LIGHT_5        "Lbs31" //  BULB 5
#define AUTO_BULB_LIGHT_5_SENS   "Lsa31" //  BULB 5 SENS

#define AUTO_BULB_LIGHT_6        "Lbs41" //  BULB 6
#define AUTO_BULB_LIGHT_6_SENS   "Lsa41" //  BULB 6 SENS

#define AUTO_BULB_LIGHT_7        "Lbs51" //  BULB 6
#define AUTO_BULB_LIGHT_7_SENS   "Lsa51" //  BULB 6 SENS

#define AUTO_BULB_LIGHT_8        "Lbs61" //  BULB 7
#define AUTO_BULB_LIGHT_8_SENS   "Lsa61" //  BULB 7 SENS

#define BULB_LIGHT9              "Lbt11" // BULB 9
#define BULB_LIGHT10             "Lbt21" // BULB 10
#define BULB_LIGHT11             "Lbt31" // BULB 11
#define BULB_LIGHT12             "Lbt41" // BULB 12
#define BULB_LIGHT13             "Lbt51" // BULB 13


//
// PRESSAIR PAGE
//
#define PA_TEXT_PRESS_CUR        "P~tcp000000"
#define PA_TEXT_PRESS_MAX        "P~tmp000000"
#define PA_TEXT_PRESS_OUT        "P~tpo000000"

#define PA_BTN_MP_ADD            "Pmpa1"
#define PA_BTN_MP_MINS           "Pmpm1"
#define PA_BTN_PO_ADD            "Ppoa1"
#define PA_BTN_PO_MINS           "Ppom1"

#define PA_BTN_SINK              "Pkds1"

#define PA_BTN_COMPRESSOR        "Pprs1"
#define PA_BTN_CONDAUTO          "Pkda1"

//
// KLIMATE PAGE
//
#define KL_TEXT_EXHAUST          "K~tex000000"
#define KL_TEXT_HEATING          "K~the000000"

#define KL_BTN_ADD_EXHAUST       "Kexa1"
#define KL_BTN_MIN_EXHAUST       "Kexm1"

#define KL_BTN_ADD_HEATING       "Khea1"
#define KL_BTN_MIN_HEATING       "Khem1"

#define KL_BTN_ADD_FAN           "Kfna1"
#define KL_BTN_MIN_FAN           "Kfnm1"


#define KL_BTN_EXHAUST_ONOFF     "KexF1"
#define KL_BTN_EXHAUST_AUTO      "KexA1"

#define KL_BTN_HEATING_ONOFF     "KheO1"

#define KL_BTN_FAN_AUTO          "KfnA1"


//
// ELECTRIK PANEL
//
#define EL_POINT_1               "Ebs11" //  POINT 1
#define EL_POINT_2               "Ebs21" //  POINT 2
#define EL_POINT_3               "Ebs31" //  POINT 3
#define EL_POINT_4               "Ebs41" //  POINT 4
#define EL_POINT_5               "Ebs51" //  POINT 5
#define EL_POINT_6               "Ebs61" //  POINT 6
#define EL_POINT_7_AUTOM         "Ebs71" //  POINT 7

#define EL_POINT_PROP_1          "Ebf11" //  POINT PROP  1
#define EL_POINT_PROP_2          "Ebf21" //  POINT PROP  2
#define EL_POINT_PROP_3          "Ebf31" //  POINT PROP  3
#define EL_POINT_PROP_4          "Ebf41" //  POINT PROP  4
//
// PAGE DOOR
//
#define  DR_LOCK_STREET_GATE     "Dgk11"
#define  DR_UNLOCK_STREET_GATE   "Dgk00"

#define  DR_LOCK_GARAGE_DOOR     "Dgd11"
#define  DR_UNLOCK_GARAGE_DOOR   "Dgd00"

#define  DR_OPEN_LUKE            "Dgl11"
#define  DR_CLOSE_LUKE           "Dgl00"

#define  DR_GARAGE_GATE_OPEN     "Dgg11"
#define  DR_GARAGE_GATE_CLOSE    "Dgg00"

#define  DR_STREET_GATE_OPEN     "Dgs11"
#define  DR_STREET_GATE_CLOSE    "Dgs00"

#endif



