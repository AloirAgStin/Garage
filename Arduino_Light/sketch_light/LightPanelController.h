#ifndef LIGHT_PANEL_H
#define LIGHT_PANEL_H

#include "LightItems.h"
#include "SerialCmd.h"

const int g_countSlotsLight = 13;


#define COUNT_USUAL_LAMP  5
#define COUNT_SENS_LAMP   8

enum AUTO_BTN { BTN_ALL = 1, BTN_CEIL, BTN_SINGLE };

class CLightPanelController
{
  friend class CMainAppl;
public:
    enum ePANEL_CMD
    {
        CMD_OFF = 0,
        CMD_ON = 1,
        CMD_AUTO = 2
    };

    CLightPanelController(CSendCmd *port);
    ~CLightPanelController(void);

    short Init();
    void Show();
    int Process();
    void ProcessSerialCMD(sCmdWord &cmd);

    void SetAutoALL(bool IsOn);
    void SetAutoSingle(bool IsOn);
    void SetAutoCeiling(bool IsOn);

    int AddSensorLight(short PIN_Light, short PIN_Sensor, unsigned int time, 
                       const char* lampName, const char* LampcmdOn, const char* LampcmdOff, 
                       const char* sensName, const char* SenscmdOn, const char* SenscmdOff);

    int AddLight(short PIN_Light, short PIN_CONVERTOR, const char* NameObj, const char *cmdOn, const char *cmdOff); 
    int AddMicroWaveSensor(short PIN_Sensor);

    bool IsActiveAnyMotionSens();
    
    bool IsRoomBulbActive();
    bool IsAnyBulbActive();


    void InvalidateScreen();
	void InvalidateA();
	void InvalidateB();



    void SendCMD(sCmdWord * cmd);

private:
    short SetupObjects();
    bool SetOnOffBulb(int ID, bool IsOn);
    
    CSendCmd* cmdArduino;



    void TurnOFFAutoBtn(AUTO_BTN n);

public:

private:
    CInverseDigitalOnOff* m_cooler;

    CMicroWaveSensor* m_MicroWaveSensor;
    CLightSwitchSensor m_arrLightSensSwitches[COUNT_SENS_LAMP];
    CLightSwitch       m_arrLightSwitches[COUNT_USUAL_LAMP];

    int m_slots[g_countSlotsLight]; //slots
    int m_UlicaLampID1;
    int m_UlicaLampID2;

    short m_CountSensLight;
    short m_CountLight;

    void ShowAutoStatus();
    bool m_isAutoAll;
      bool m_IsAutoCeiling;
      bool m_isAutoSingle;

};

#endif
