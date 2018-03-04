#include "ElecLightPanelController.h"
#include "MainAppl.h"

extern CMainAppl *app;

CElecLightPanelController::CElecLightPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd) 
    : CNexPageEx(pageID,objID,pageName),
      m_SerialCmd(_SerialCmd)
{
    m_countBtns = 0;
    m_countBtnsProp = 0;

    m_btns = 0;
    m_btnsProp = 0;
}

CElecLightPanelController::~CElecLightPanelController(void)
{
    delete_2x_Array(m_btns, m_countBtns)
    delete_2x_Array(m_btnsProp, m_countBtnsProp)
}

void CElecLightPanelController::Init()
{
    m_currListenItem = 0;
    for(int i = 0 ; i < g_countNextListenItemSocet; i++)
        m_nex_listen_list[i] = 0;

    m_btns = new CNextionDSObject*[7];
    AddSocketBtn(m_pageID, 10, "bs1", EL_POINT_1, "");
    AddSocketBtn(m_pageID, 13, "bs2", EL_POINT_2, "");
    AddSocketBtn(m_pageID, 15, "bs3", EL_POINT_3, "");    
    AddSocketBtn(m_pageID, 16, "bs4", EL_POINT_4, "");
    AddSocketBtn(m_pageID, 18, "bs5", EL_POINT_5, "");
    AddSocketBtn(m_pageID, 19, "bs6", EL_POINT_6, "");
    AddSocketBtn(m_pageID, 20, "bs7", EL_POINT_7_AUTOM, "");


    m_btnsProp = new CNextionDSObject*[4];
    AddSocketPropBtn(m_pageID, 11, "bf1", EL_POINT_PROP_1, "");
    AddSocketPropBtn(m_pageID, 12, "bf2", EL_POINT_PROP_2, "");
    AddSocketPropBtn(m_pageID, 14, "bf3", EL_POINT_PROP_3, "");
    AddSocketPropBtn(m_pageID, 17, "bf4", EL_POINT_PROP_4, "");

    int otherButtonCmd = 1;
    m_btnMenu = new NexButton(m_pageID, 1, "bMenu");  
    m_btnMenu->attachPush(CElecLightPanelController::EventPageButton, (void*)otherButtonCmd);
    m_nex_listen_list[m_currListenItem++] = m_btnMenu;
    

    m_pDate    = new NexText(m_pageID, 5, "tDate");
    m_pTime    = new NexText(m_pageID, 4, "tTime");
    m_pTempIn  = new NexText(m_pageID, 3, "tTpO");
    m_pTempOut = new NexText(m_pageID, 2, "tTpI");
    m_pPress   = new NexText(m_pageID, 8, "tPress");
    m_pPower   = new NexText(m_pageID, 7, "tPower");
    m_pVoltage = new NexText(m_pageID, 6,  "tVolt");
    m_pAmpers  = new NexText(m_pageID, 9, "tAmper");
}


void CElecLightPanelController::AddSocketBtn(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_btns[m_countBtns] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* lamp = m_btns[m_countBtns];
    
    lamp->GetNextObj()->attachPush(CElecLightPanelController::EventOnOFFBtn, lamp);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();

    m_countBtns++;
}

void CElecLightPanelController::AddSocketPropBtn(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_btnsProp[m_countBtnsProp] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* lamp = m_btnsProp[m_countBtnsProp];
    
    lamp->GetNextObj()->attachPush(CElecLightPanelController::EventOnOFFBtn, lamp);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();

    m_countBtnsProp++;
}

void CElecLightPanelController::Show()
{
    show();// show page
}

void CElecLightPanelController::EventOnOFFBtn(void *ptr)
{
  CNextionDSObject *object = (CNextionDSObject*)ptr;
  
  uint32_t value = 0;
  object->m_button.getValue(&value);
  object->m_oldValue = value;
  
  CElecLightPanelController* pController = (CElecLightPanelController*)app->m_Constructor.GetObject(eElectNetWork);
  pController->m_SerialCmd->SendCommand(object->GetCmdByValue(value));
}

void CElecLightPanelController::EventPageButton(void *ptr)
{
  int n = (int)ptr;

  if(n == 1)        //go to main menu
    app->Show();
}

void CElecLightPanelController::ProcessCMD(sCmdWord &cmd)
{
    int action = cmd.GetAction();

    if(cmd.IsCommand(SHOW_MAIN_MENU))
        app->Show();
    else
    if(cmd.buff[2] == 's')
    {
        for(int i = 0; i < m_countBtns; i++)
        {    
            if(cmd.IsName(m_btns[i]->m_name))
            {
                m_btns[i]->SetValue(action);
                return;
            }
        }
    }
    else
    {
        for(int i = 0; i < m_countBtnsProp; i++)
        {    
            if(cmd.IsName(m_btnsProp[i]->m_name))
            {
                m_btnsProp[i]->SetValue(action);
                return;
            }
        }
    }
}


void CElecLightPanelController::ProcessInvalidateCMD(sCmdInvalidate& cmd)
{
	//invalidte sens lamp
	short nextPropBtn = -1;
	short nextBtn = -1;

	for(int i = 0; i < cmd.GetCountItems(); i++)
	{
		int val = cmd.GetItem(i) == '1' ? 1 : 0;
		
		if( i == 1 || i == 3 || i == 5 || i == 7)
		{
			nextPropBtn++;
			m_btnsProp[nextPropBtn]->SetValue(val);
		}
		else
		{
			nextBtn++;
			m_btns[nextBtn]->SetValue(val);
		}
		delay(30);
	}
}


//------------------------------------------------------------------------

