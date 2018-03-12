#pragma once
#ifdef WIN32
#include "..\Nextion.h"
#else
    #include "Nextion.h"
#endif
#include "NextionObject.h"
#include "SerialCmd.h"

const int g_countNextListenItemSocet = 12 + 1;

class CElecLightPanelController : public CNexPageEx
{
    friend class CMainAppl;

public:

    CElecLightPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd);
    ~CElecLightPanelController(void);

    virtual void Init();
    virtual void Show();

    virtual void ProcessInterface() { nexLoop(m_nex_listen_list); } 
    virtual void ProcessCMD(sCmdWord &cmd);
	virtual void ProcessInvalidateCMD(sCmdInvalidate& cmd);

  
    static void EventOnOFFBtn(void *ptr);
    static void EventPageButton(void *ptr);

    void AddSocketBtn(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF);
    void AddSocketPropBtn(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF);
  
    CNextionDSObject **m_btns;
    short m_countBtns;

    CNextionDSObject **m_btnsProp;
    short m_countBtnsProp;
  
public:
    //reference to other object
    CSendCmd * m_SerialCmd;

    NexTouch* m_nex_listen_list[g_countNextListenItemSocet];
    short m_currListenItem;
};




