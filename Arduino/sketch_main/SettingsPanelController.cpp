#include "ElecLightPanelController.h"
#include "MainAppl.h"

extern CMainAppl *app;

CSettingsPanelController::CSettingsPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd) 
    : CNexPageEx(pageID,objID,pageName),
      m_SerialCmd(_SerialCmd)
{
    m_countBtns = 0;
    m_countBtnsProp = 0;

    m_btns = 0;
    m_btnsProp = 0;
}

CSettingsPanelController::~CSettingsPanelController(void)
{
    delete_2x_Array(m_btns, m_countBtns)
    delete_2x_Array(m_btnsProp, m_countBtnsProp)
}

void CSettingsPanelController::Init()
{
    m_currListenItem = 0;
    for(int i = 0 ; i < g_countNextListenItemSett; i++)
        m_nex_listen_list[i] = 0;

//    m_btns = new CNextionDSObject*[7];
//    AddSocketBtn(m_pageID, 18, "bs1", EL_POINT_1_ON, EL_POINT_1_OFF);   
    
//    m_btnsProp = new CNextionDSObject*[4];
//    AddSocketPropBtn(m_pageID, 19, "bf1", EL_POINT_PROP_1_ON, EL_POINT_PROP_1_OFF);

    int otherButtonCmd = 1;
    m_btnMenu = new NexButton(m_pageID, 1, "bMenu");  
    m_btnMenu->attachPush(CSettingsPanelController::EventPageButton, (void*)otherButtonCmd);
    m_nex_listen_list[m_currListenItem++] = m_btnMenu;
    
    m_pDate    = new NexText(m_pageID, 7, "tDate");
    m_pTime    = new NexText(m_pageID, 6, "tTime");
    m_pTempIn  = new NexText(m_pageID, 5, "tTpO");
    m_pTempOut = new NexText(m_pageID, 4, "tTpI");
    m_pPress   = new NexText(m_pageID, 13, "tPress");
    m_pPower   = new NexText(m_pageID, 10, "tPower");
    m_pVoltage = new NexText(m_pageID, 8,  "tVolt");
    m_pAmpers  = new NexText(m_pageID, 15, "tAmper");

    delay(50);
}

/*
void CSettingsPanelController::AddSocketBtn(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_btns[m_countBtns] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* lamp = m_btns[m_countBtns];
    
    lamp->GetNextObj()->attachPush(CSettingsPanelController::EventOnOFFBtn, lamp);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();

    m_countBtns++;
}

void CSettingsPanelController::AddSocketPropBtn(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_btnsProp[m_countBtnsProp] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* lamp = m_btnsProp[m_countBtnsProp];
    
    lamp->GetNextObj()->attachPush(CSettingsPanelController::EventOnOFFBtn, lamp);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();

    m_countBtnsProp++;
}
*/

void CSettingsPanelController::Show()
{
    show();// show page
}
/*
void CSettingsPanelController::EventOnOFFBtn(void *ptr)
{
  CNextionDSObject *lamp = (CNextionDSObject*)ptr;
  
  uint32_t value = 0;
  lamp->m_button.getValue(&value);
  lamp->m_oldValue = value;
  
  CSettingsPanelController* pController = (CSettingsPanelController*)app->m_Constructor.GetObject(eElectNetWork);
  pController->m_SerialCmd->SendCommand(value == 1 ? lamp->m_cmdON : lamp->m_cmdOFF);
}
*/

void CSettingsPanelController::EventPageButton(void *ptr)
{
  int n = (int)ptr;

  if(n == 1)        //go to main menu
    app->Show();
}

void CSettingsPanelController::ProcessCMD(sCmdWord &cmd)
{
    int action = cmd.GetAction();

    /*
    if(cmd.buff[2] == 'b' && cmd.buff[3] == 's')
    {
        char targetname[SIZE_OFNEXT_OBJECT_NAME]={0};
        for(int i = 0; i < m_countBtns; i++)
        {    
            if(strstr(cmd.buff, m_btns[i]->m_name) != 0)
            {
                m_btns[i]->SetValue(action);
                break;
            }
        }
    }
    else
    if(cmd.buff[2] == 'b' && cmd.buff[3] == 'f')
    {
        char targetname[SIZE_OFNEXT_OBJECT_NAME]={0};
        for(int i = 0; i < m_countBtnsProp; i++)
        {    
            if(strstr(cmd.buff, m_btnsProp[i]->m_name) != 0)
            {
                m_btnsProp[i]->SetValue(action);
                break;
            }
        }
    }
    else
        */
    if(cmd.IsCommand(SHOW_MAIN_MENU))
        app->Show();
}


//------------------------------------------------------------------------

