#pragma once
#ifdef WIN32
#include "..\Nextion.h"
#else
#include "Nextion.h"
#endif
#include "NextionObject.h"
#include "SerialCmd.h"

const int g_countNextListenItemKlimate = 11 + 1;

class CKlimatePanelController : public CNexPageEx
{
    friend class CMainAppl;

public:

    CKlimatePanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd);
    ~CKlimatePanelController(void);

    virtual void Init();
    virtual void Show();

    virtual void ProcessInterface() { nexLoop(m_nex_listen_list); } 
    virtual void ProcessCMD(sCmdWord &cmd);
	virtual void ProcessInvalidateCMD(sCmdInvalidate& cmd);


    static void EventPageButton(void *ptr);

    void AddObject(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmd);
    static void EventPressButton(void *ptr);

    void AddObjectDS(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF);
    static void EventPressDSButton(void *ptr);


    //labels with text data
    NexText **m_txtObject;
    short m_countTxtObject;

    //buttons 
    CNextionSingleObject **m_Btns;
    short m_countBtns;

    CNextionDSObject     **m_dsBtns;
    short m_countDSBtns;

public:
    //reference to other object
    CSendCmd * m_SerialCmd;

    NexTouch* m_nex_listen_list[g_countNextListenItemKlimate];
    short m_currListenItem;
};
