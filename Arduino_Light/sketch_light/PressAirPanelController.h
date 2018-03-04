#ifndef PRESSAIR_PANEL_H
#define PRESSAIR_PANEL_H

#include "SerialCmd.h"
#include "BasesClasses.hpp"
#include "PressAirElem.hpp"

class CPressAirPanelController
{
  friend class CMainAppl;
public:
    enum ePANEL_CMD
    {
        CMD_OFF = 0,
        CMD_ON = 1,
        CMD_AUTO = 2
    };

    CPressAirPanelController(CSendCmd *port);
    ~CPressAirPanelController(void);

    short Init();
    
    void Show();
    void InvalidateScreen();

    int Process();
    void ProcessSerialCMD(sCmdWord &cmd);
    
    void SendCMD(sCmdWord* cmd);

private:
    short SetupObjects();    
    bool SetOnOffBulb(int ID, bool IsOn);

public:

private:
  CCompressor m_compressor;
    CSinkBtn    m_sink;
    CAnalogReader m_pressSens;

    CSendCmd*    cmdArduino;  
    

    bool readPressAir(short num, float &value, bool _force = false);
    bool setPress(short num, float value);

    float m_currPress;
    float m_maxPress;
    float m_pressOut;
};

#endif
