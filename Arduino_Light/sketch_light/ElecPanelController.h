#ifndef ELECT_PANEL_H
#define ELECT_PANEL_H

#include "ElecItems.h"
#include "SerialCmd.h"

#define COUNT_SOCKET 7
static const int g_countSlots = 13;

class CElecPanelController
{
  friend class CMainAppl;
public:
    enum ePANEL_CMD
    {
        CMD_OFF = 0,
        CMD_ON = 1,
        CMD_AUTO = 2
    };

    CElecPanelController(CSendCmd *port);
    ~CElecPanelController(void);

    short Init();
    
    void Show();
    void InvalidateScreen();

    int Process();
    void ProcessSerialCMD(sCmdWord &cmd);
    
    void SendCMD(sCmdWord* cmd);

    bool IsAnySocketON();
    void TurnOFFCooler();

    bool IsIndicatorMediaSystemON(); 

private:
    short SetupObjects();    
    bool SetOnOffBulb(int ID, bool IsOn);
public:

    static const short m_maxSocetCount;

private:
    CSimpleSocet **m_arrSocketSwitches;

    CMultimediaSystem *m_mediaSystem;
    CDigitalSensor* m_VoltageReader;
    CInverseDigitalOnOff* m_cooler;




    CSendCmd* cmdArduino;

    int m_slots[g_countSlots]; //slots
    short m_countSocket;    
};

#endif
