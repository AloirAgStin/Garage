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

#define SHOW_MAIN_MENU           "$MENUL1000000001~" //GO TO MAIN MENU

#define CMD_SETBRT               "$#SETBRT00000000~" //GO TO MAIN MENU
#define CMD_SLEEP                "$#SLEEP000000001~"  //GO TO MAIN MENU

//
//MAINPAGECMD
//
#define SHOW_LIGHT_PAGE          "$LIG000000000001~" //SHOW LIGHT PAGE, ASK FOR INVALIDATE
#define SHOW_PRESSAIR_PAGE       "$PRES00000000001~" //SHOW PRESSAIR PAGE, ASK FOR INVALIDATE
#define SHOW_KLIMATE_PAGE        "$KLIM00000000001~" //SHOW KLIMATE PAGE, ASK FOR INVALIDATE
#define SHOW_ELECT_NET_PAGE      "$ENET00000000001~" //SHOW ELECTRIC PAGE, ASK FOR INVALIDATE
#define SHOW_DOOR_PAGE           "$DOOR00000000001~" //SHOW DOOR PAGE, ASK FOR INVALIDATE
#define SHOW_SETTINGS_PAGE       "$SETT00000000001~" //SHOW SETTINGS PAGE, ASK FOR INVALIDATE

//LIGHT PAGE

#define BTN_AUTO_ALL             "$LAUTO0000000001~"  // BTN AUTO ALL
    

#define AUTO_BULB_LIGHT_1        "$LbsGat000000001~" //  BULB 2
#define AUTO_BULB_LIGHT_1_SENS   "$LbsaGa000000001~" //  BULB 2 SENS

#define AUTO_BULB_LIGHT_2        "$LbsDoo000000001~" //  BULB 2
#define AUTO_BULB_LIGHT_2_SENS   "$LbsaDo000000001~" //  BULB 2 SENS

#define AUTO_BULB_LIGHT_3        "$Lbs100000000001~" //  BULB 3
#define AUTO_BULB_LIGHT_3_SENS   "$Lbsa10000000001~" //  BULB 3 SENS

#define AUTO_BULB_LIGHT_4        "$Lbs200000000001~" //  BULB 4
#define AUTO_BULB_LIGHT_4_SENS   "$Lbsa20000000001~" //  BULB 4 SENS

#define AUTO_BULB_LIGHT_5        "$Lbs300000000001~" //  BULB 5
#define AUTO_BULB_LIGHT_5_SENS   "$Lbsa30000000001~" //  BULB 5 SENS

#define AUTO_BULB_LIGHT_6        "$Lbs400000000001~" //  BULB 6
#define AUTO_BULB_LIGHT_6_SENS   "$Lbsa40000000001~" //  BULB 6 SENS

#define AUTO_BULB_LIGHT_7        "$Lbs500000000001~" //  BULB 6
#define AUTO_BULB_LIGHT_7_SENS   "$Lbsa50000000001~" //  BULB 6 SENS

#define AUTO_BULB_LIGHT_8        "$Lbs600000000001~" //  BULB 7
#define AUTO_BULB_LIGHT_8_SENS   "$Lbsa60000000001~" //  BULB 7 SENS


#define BULB_LIGHT9              "$Lbt100000000001~" // BULB 9
#define BULB_LIGHT10             "$Lbt200000000001~" // BULB 10
#define BULB_LIGHT11             "$Lbt300000000001~" // BULB 11
#define BULB_LIGHT12             "$Lbt400000000001~" // BULB 12
#define BULB_LIGHT13             "$Lbt500000000001~" // BULB 13


//
// PRESSAIR PAGE
//
#define PA_TEXT_PRESS_CUR        "$Ptxtcp000000000~"
#define PA_TEXT_PRESS_MAX        "$Ptxtmp000000000~"
#define PA_TEXT_PRESS_OUT        "$Ptxtpo000000000~"

#define PA_BTN_MP_ADD            "$Pmpadd000000001~"
#define PA_BTN_MP_MINS           "$Pmpmin000000001~"
#define PA_BTN_PO_ADD            "$Ppoadd000000001~"
#define PA_BTN_PO_MINS           "$Ppomin000000001~"

#define PA_BTN_SINK              "$Pkds00000000001~"

#define PA_BTN_COMPRESSOR        "$Ppres0000000001~"
#define PA_BTN_CONDAUTO          "$Pkda00000000001~"

//
// KLIMATE PAGE
//
#define KL_TEXT_EXHAUST          "$Ktxtex000000000~"
#define KL_TEXT_HEATING          "$Ktxthe000000000~"

#define KL_BTN_ADD_EXHAUST       "$Kexadd000000001~"
#define KL_BTN_MIN_EXHAUST       "$Kexmin000000001~"

#define KL_BTN_ADD_HEATING       "$Kheadd000000001~"
#define KL_BTN_MIN_HEATING       "$Khemin000000001~"

#define KL_BTN_ADD_FAN           "$Kfnadd000000001~"
#define KL_BTN_MIN_FAN           "$Kfnmin000000001~"


#define KL_BTN_EXHAUST_ONOFF     "$KexON0000000001~"
#define KL_BTN_EXHAUST_AUTO      "$KexAU0000000001~"

#define KL_BTN_HEATING_ONOFF     "$KheON0000000001~"

#define KL_BTN_FAN_AUTO          "$KfnAU0000000001~"


//
// ELECTRIK PANEL
//
#define EL_POINT_1               "$Ebs100000000001~" //  POINT 1
#define EL_POINT_2               "$Ebs200000000001~" //  POINT 2
#define EL_POINT_3               "$Ebs300000000001~" //  POINT 3
#define EL_POINT_4               "$Ebs400000000001~" //  POINT 4
#define EL_POINT_5               "$Ebs500000000001~" //  POINT 5
#define EL_POINT_6               "$Ebs600000000001~" //  POINT 6

#define EL_POINT_7_AUTOM         "$Ebs700000000001~" //  POINT 7

#define EL_POINT_PROP_1          "$Ebf100000000001~" //  POINT PROP  1
#define EL_POINT_PROP_2          "$Ebf200000000001~" //  POINT PROP  2
#define EL_POINT_PROP_3          "$Ebf300000000001~" //  POINT PROP  3
#define EL_POINT_PROP_4          "$Ebf400000000001~" //  POINT PROP  4
#define EL_POINT_PROP_5          "$Ebf500000000001~" //  POINT PROP  5
#define EL_POINT_PROP_6          "$Ebf600000000001~" //  POINT PROP  6

//
// PAGE DOOR
//
#define  DR_LOCK_STREET_GATE     "$Dgk100000000001~"
#define  DR_UNLOCK_STREET_GATE   "$Dgk000000000000~"

#define  DR_LOCK_GARAGE_DOOR     "$Dgd100000000001~"
#define  DR_UNLOCK_GARAGE_DOOR   "$Dgd000000000000~"

#define  DR_OPEN_LUKE            "$Dgl100000000001~"
#define  DR_CLOSE_LUKE           "$Dgl000000000000~"

#define  DR_GARAGE_GATE_OPEN     "$Dgg100000000001~"
#define  DR_GARAGE_GATE_CLOSE    "$Dgg000000000000~"

#define  DR_STREET_GATE_OPEN     "$Dgs100000000001~"
#define  DR_STREET_GATE_CLOSE    "$Dgs100000000000~"

#endif



