#pragma once
#ifdef WIN32
#include "..\Nextion.h"
#else
    #include "Nextion.h"
#endif
#include "NextionObject.h"
#include "SerialCmd.h"

const int g_countNextListenItemDoor = 11 + 1;

class CDoorPanelController : public CNexPageEx
{
    friend class CMainAppl;

public:

    CDoorPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd);
    ~CDoorPanelController(void);

    virtual void Init();
    virtual void Show();

    virtual void ProcessInterface() { nexLoop(m_nex_listen_list); } 
    virtual void ProcessCMD(sCmdWord &cmd);
	virtual void ProcessInvalidateCMD(sCmdInvalidate& cmd){}

    static void EventPageButton(void *ptr);
  

    void AddObject(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmd);
    static void EventPressButton(void *ptr);

    //buttons 
    CNextionSingleObject **m_Btns;
    short m_countBtns;
  
public:
    //reference to other object
    CSendCmd * m_SerialCmd;

    NexTouch* m_nex_listen_list[g_countNextListenItemDoor];
    short m_currListenItem;
};




