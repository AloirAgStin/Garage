#pragma once
#ifdef WIN32
#include "..\Nextion.h"
#else
    #include "Nextion.h"
#endif
#include "SerialCmd.h"

#define SIZE_OFNEXT_OBJECT_NAME 16


class CNextionDSObject
{
public:
    CNextionDSObject(uint8_t pid, uint8_t cid, const char *_name, const char* cmdON, const char* cmdOFF = "");
    virtual ~CNextionDSObject(void);

    NexTouch* GetNextObj() { return &m_button; } 
    bool m_IsTypeOnOff;

    void SetValue(uint32_t _val);

    void Invalidate();
    
    sCmdWord* GetCmdByValue(uint32_t val);

    uint32_t m_oldValue;
    
    char m_name[SIZE_OFNEXT_OBJECT_NAME];
    
    sCmdWord *m_cmdON;
    sCmdWord *m_cmdOFF;

    NexDSButton m_button;
};


class CNextionSingleObject
{
public:
    CNextionSingleObject(uint8_t pid, uint8_t cid, const char *_name, const char* cmd);
    virtual ~CNextionSingleObject(void);

    NexTouch* GetNextObj() { return &m_button; }
    bool SetColor(uint32_t color) { return m_button.Set_background_color_bco(color); }

    char m_name[SIZE_OFNEXT_OBJECT_NAME];
    sCmdWord *m_cmd;
    
    NexButton m_button;
};



class CNexPageEx: public NexPage
{
public:
    CNexPageEx(uint8_t pid, uint8_t cid, const char *name);
    virtual ~CNexPageEx();
    
    virtual void Show()  = 0;
    virtual void Init() = 0;
    virtual void ProcessInterface() = 0;    
    virtual void ProcessCMD(sCmdWord & cmd) = 0;
	virtual void ProcessInvalidateCMD(sCmdInvalidate& cmd) = 0;


    uint8_t   m_pageID; // pageID
    NexButton *m_btnMenu; //button to menu

    NexText* m_pDate;
    NexText* m_pTime;
    NexText* m_pTempIn;
    NexText* m_pTempOut;
    NexText* m_pPress;
    NexText* m_pPower;
    NexText* m_pVoltage;
    NexText* m_pAmpers;
};



