#pragma once
#ifdef WIN32
#include "..\Nextion.h"
#else
    #include "Nextion.h"
#endif

#include "NextionObject.h"
#include "SerialCmd.h"

const int g_countNextListenItemLight = 25 + 1; 

class CLightPanelController : public CNexPageEx
{
   friend class CMainAppl;
public:

    CLightPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd);
    ~CLightPanelController(void);

    virtual void Init();
    virtual void Show();

    virtual void ProcessInterface() { nexLoop(m_nex_listen_list);} 
    virtual void ProcessCMD(sCmdWord &cmd);
	virtual void ProcessInvalidateCMD(sCmdInvalidate& cmd);


    static void EventOnOFFBtn(void *ptr);
    static void EventPageButton(void *ptr);

    static void EventSensorOnOFF(void *ptr);
    static void EventSensorButton(void *ptr);

    void AddLight(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF);
    void AddSensorLight(uint8_t pid, uint8_t LampID, const char* lampName, const char* LampCmdON,const char* LampCmdOFF, 
                        uint8_t sensorID, const char* sensName, const char* SensCmdON,const char* SensCmdOFF);

    CNextionDSObject *m_btnAutoAll;
    CNextionDSObject *m_btnAutoSingle;
    CNextionDSObject *m_btnAutoCeil;

    
    CNextionDSObject **m_btns;
    short m_countBtns;

    CNextionDSObject **m_btnsSens;
    short m_countBtnsSens;

public:
    CSendCmd * m_SerialCmd;
    
    NexTouch* m_nex_listen_list[g_countNextListenItemLight];
    short m_currListenItem;
};




