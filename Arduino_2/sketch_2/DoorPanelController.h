#ifndef LIGHT_PANEL_H
#define LIGHT_PANEL_H

#include "SerialCmd.h"

const int g_countSlotsLight = 13;


#define COUNT_USUAL_LAMP  5
#define COUNT_SENS_LAMP   8

class CDoorPanelController
{
  friend class CMainAppl;
public:
    enum ePANEL_CMD
    {
        CMD_OFF = 0,
        CMD_ON = 1,
        CMD_AUTO = 2
    };

    CDoorPanelController(CSendCmd *port);
    ~CDoorPanelController(void);

    short Init();
    short SetupObjects();
	void Show();
    int Process();
    void ProcessSerialCMD(sCmdWord &cmd);
	

    void SendCMD(sCmdWord * cmd);

private:
   
    CSendCmd* cmdArduino;

public:

private:
};

#endif
