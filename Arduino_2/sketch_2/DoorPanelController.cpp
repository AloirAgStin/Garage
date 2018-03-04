#include "DoorPanelController.h"

#include "SerialController.hpp"

#define CMD_DELAY 50 //delay between command (this page)

short CDoorPanelController::SetupObjects()
{   
    return 1;
}



int CDoorPanelController::Process()
{
 
    return 1;
}

void CDoorPanelController::SendCMD(sCmdWord* cmd)
{
    if(CSerialController::ActivepaPage != eDoor)
        return;

    cmdArduino->SendCommand(cmd);
}

void CDoorPanelController::ProcessSerialCMD(sCmdWord &cmd)
{
    
}

void CDoorPanelController::Show()
{
    //InvalidateScreen();
}


CDoorPanelController::CDoorPanelController(CSendCmd *port) 
{
}


short CDoorPanelController::Init()
{
 
    return 1;
}

CDoorPanelController::~CDoorPanelController(void)
{
}
