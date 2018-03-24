#include "LightPanelController.h"

#include "MainAppl.h"

CLightPanelController::CLightPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd) 
    : CNexPageEx(pageID,objID,pageName),
    m_SerialCmd(_SerialCmd)
{
    m_countBtns = 0;
    m_countBtnsSens = 0;
     
    m_btns = 0;
    m_btnsSens = 0;
}

CLightPanelController::~CLightPanelController(void)
{
    delete_if_exists(m_btnAutoAll);
    delete_if_exists(m_btnAutoSingle);
    delete_if_exists(m_btnAutoCeil);

    delete_2x_Array(m_btns, m_countBtns)
    delete_2x_Array(m_btnsSens, m_countBtnsSens)
}

void CLightPanelController::Init()
{   
    SMESN("INIT LIGHTPANE");
    
    m_currListenItem = 0;
    for(int i = 0 ; i < g_countNextListenItemLight; i++)
        m_nex_listen_list[i] = 0;

    m_btnsSens = new CNextionDSObject*[16]; //count * 2
    AddSensorLight(m_pageID, 14, "bsG", AUTO_BULB_LIGHT_1,  "", 23, "saG", AUTO_BULB_LIGHT_1_SENS, "");
    AddSensorLight(m_pageID, 13, "bsD", AUTO_BULB_LIGHT_2,  "", 22, "saD", AUTO_BULB_LIGHT_2_SENS, "");
    AddSensorLight(m_pageID, 7,  "bs1", AUTO_BULB_LIGHT_3,  "", 16, "sa1", AUTO_BULB_LIGHT_3_SENS, "");
    AddSensorLight(m_pageID, 9,  "bs2", AUTO_BULB_LIGHT_4,  "", 17, "sa2", AUTO_BULB_LIGHT_4_SENS, "");
    AddSensorLight(m_pageID, 11, "bs3", AUTO_BULB_LIGHT_5,  "", 18, "sa3", AUTO_BULB_LIGHT_5_SENS, "");
    AddSensorLight(m_pageID, 8,  "bs4", AUTO_BULB_LIGHT_6,  "", 19, "sa4", AUTO_BULB_LIGHT_6_SENS, "");
    AddSensorLight(m_pageID, 10, "bs5", AUTO_BULB_LIGHT_7,  "", 20, "sa5", AUTO_BULB_LIGHT_7_SENS, "");
    AddSensorLight(m_pageID, 12, "bs6", AUTO_BULB_LIGHT_8,  "", 21, "sa6", AUTO_BULB_LIGHT_8_SENS, "");

    m_btns = new CNextionDSObject*[5]; //count
    AddLight(m_pageID, 3, "bt1", BULB_LIGHT9,  "");
    AddLight(m_pageID, 4, "bt2", BULB_LIGHT10, "");
    AddLight(m_pageID, 1, "bt3", BULB_LIGHT11, "");
    AddLight(m_pageID, 2, "bt4", BULB_LIGHT12, "");
    AddLight(m_pageID, 5, "bt5", BULB_LIGHT13, "");

    int nCommand = 1;
    m_btnMenu = new NexButton(m_pageID, 6, "bMenu");  
    m_btnMenu->attachPush(CLightPanelController::EventPageButton, (void*) nCommand);
    m_nex_listen_list[m_currListenItem++] = m_btnMenu;

    nCommand = 2;
    m_btnAutoAll = new CNextionDSObject(m_pageID, 15, "bsAutroAll",BTN_AUTO_ALL, "");
    m_btnAutoAll->m_IsTypeOnOff = false;
    m_btnAutoAll->m_button.attachPush(CLightPanelController::EventPageButton, (void*) nCommand);
    m_nex_listen_list[m_currListenItem++] = m_btnAutoAll->GetNextObj();

    nCommand = 3;
    m_btnAutoSingle = new CNextionDSObject(m_pageID, 33, "bsAuS",BTN_AUTO_SING, "");
    m_btnAutoSingle->m_IsTypeOnOff = false;
    m_btnAutoSingle->m_button.attachPush(CLightPanelController::EventPageButton, (void*) nCommand);
    m_nex_listen_list[m_currListenItem++] = m_btnAutoSingle->GetNextObj();

    nCommand = 4;
    m_btnAutoCeil = new CNextionDSObject(m_pageID, 32, "bsAuC",BTN_AUTO_CEIL, "");
    m_btnAutoCeil->m_IsTypeOnOff = false;
    m_btnAutoCeil->m_button.attachPush(CLightPanelController::EventPageButton, (void*) nCommand);
    m_nex_listen_list[m_currListenItem++] = m_btnAutoCeil->GetNextObj();
    
    m_pDate        = new NexText(m_pageID, 27, "tDate");
    m_pTime        = new NexText(m_pageID, 26, "tTime");
    m_pTempIn      = new NexText(m_pageID, 25, "tTpO");
    m_pTempOut     = new NexText(m_pageID, 24, "tTpI");
    m_pPress       = new NexText(m_pageID, 30, "tPress");
    m_pPower       = new NexText(m_pageID, 29, "tPower");
    m_pVoltage     = new NexText(m_pageID, 28, "tVolt");
    m_pAmpers      = new NexText(m_pageID, 31, "tAmper");
}

void CLightPanelController::AddSensorLight(uint8_t pid, uint8_t LampID, const char* lampName, const char* LampCmdON,const char* LampCmdOFF, 
      uint8_t sensorID, const char* sensName, const char* SensCmdON,const char* SensCmdOFF)
{    
    //lamp
    m_btnsSens[m_countBtnsSens] = new CNextionDSObject(pid, LampID, lampName, LampCmdON, LampCmdOFF);
    CNextionDSObject* lamp = m_btnsSens[m_countBtnsSens];
    
    lamp->GetNextObj()->attachPush(CLightPanelController::EventSensorOnOFF, (void*)m_countBtnsSens);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();
    m_countBtnsSens++;

    //sensor  
    m_btnsSens[m_countBtnsSens] = new CNextionDSObject(pid, sensorID, sensName, SensCmdON, SensCmdOFF);
    lamp = m_btnsSens[m_countBtnsSens];
     
    lamp->GetNextObj()->attachPush(CLightPanelController::EventSensorButton, (void*)m_countBtnsSens);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();
    m_countBtnsSens++;
}


void CLightPanelController::AddLight(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_btns[m_countBtns] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* lamp = m_btns[m_countBtns];
    
    lamp->m_IsTypeOnOff = true;
    lamp->GetNextObj()->attachPush(CLightPanelController::EventOnOFFBtn, lamp);
    m_nex_listen_list[m_currListenItem++] = lamp->GetNextObj();

    m_countBtns++;
}

//------------------------------------------------------------------------
extern CMainAppl *app;

void CLightPanelController::Show()
{
    show();// show light page    
}

void CLightPanelController::EventOnOFFBtn(void *ptr)
{
    CNextionDSObject *lamp = (CNextionDSObject*)ptr;

    uint32_t value = 0;
    lamp->m_button.getValue(&value);
    lamp->m_oldValue = value;
    
    CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);
    pController->m_SerialCmd->SendCommand(lamp->GetCmdByValue(value));
}

void CLightPanelController::EventPageButton(void *ptr)
{
    int n = (int)ptr;
    if(n == 1) //go to main menu
        app->Show();
    else
    if(n == 2) // auto all
    {
        uint32_t value = 0;

        CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);
        pController->m_btnAutoAll->m_button.getValue(&value);
        pController->m_btnAutoAll->m_oldValue = value;

        pController->m_SerialCmd->SendCommand(pController->m_btnAutoAll->GetCmdByValue(value));
    }
      else
    if(n == 3) // auto single
    {
        uint32_t value = 0;

        CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);
        pController->m_btnAutoSingle->m_button.getValue(&value);
        pController->m_btnAutoSingle->m_oldValue = value;

        pController->m_SerialCmd->SendCommand(pController->m_btnAutoSingle->GetCmdByValue(value));
     }
     else
     if(n == 4) // auto ceil
     {
        uint32_t value = 0;

        CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);
        pController->m_btnAutoCeil->m_button.getValue(&value);
        pController->m_btnAutoCeil->m_oldValue = value;

        pController->m_SerialCmd->SendCommand(pController->m_btnAutoCeil->GetCmdByValue(value));
      }
}

void CLightPanelController::EventSensorOnOFF(void *ptr)
{
    int num = (int)ptr;
    
    CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);
    
    CNextionDSObject *lamp = pController->m_btnsSens[num];
    CNextionDSObject *lampSensor = pController->m_btnsSens[num+1];

    uint32_t value = 0;
    lamp->m_button.getValue(&value);
    lamp->m_oldValue = value;
    pController->m_SerialCmd->SendCommand(lamp->GetCmdByValue(value));

    lampSensor->SetValue(0);
}

void CLightPanelController::EventSensorButton(void *ptr)
{
    int num = (int)ptr;

    CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);

    CNextionDSObject *lampSensor = pController->m_btnsSens[num];
    CNextionDSObject *lamp       = pController->m_btnsSens[num-1];
    
    uint32_t value = 0;
    lampSensor->m_button.getValue(&value);
    lampSensor->m_oldValue = value;

    pController->m_SerialCmd->SendCommand(lampSensor->GetCmdByValue(value));

    lamp->SetValue(0);
}

void CLightPanelController::ProcessInvalidateCMD(sCmdInvalidate& cmd)
{
	if(cmd.GetMode() == 'A')
	{
		//invalidte sens lamp
		for(int i = 0; i < cmd.GetCountItems(); i++)
		{
			m_btnsSens[i]->SetValue(cmd.GetItem(i) == '1' ? 1 : 0);
			delay(30);
		}
	}
	else
	if(cmd.GetMode() == 'B')
	{
		int i = 0;
		for(; i < m_countBtns; i++)
		{
			m_btns[i]->SetValue(cmd.GetItem(i) == '1' ? 1 : 0);
			delay(30);
		}

		m_btnAutoAll->SetValue(cmd.GetItem(i++) == '1' ? 1 : 0);
		delay(30);
		
		m_btnAutoCeil->SetValue(cmd.GetItem(i++) == '1' ? 1 : 0);
		delay(30);
		
		m_btnAutoSingle->SetValue(cmd.GetItem(i++) == '1' ? 1 : 0);
		delay(30);
	}
}

void CLightPanelController::ProcessCMD(sCmdWord &cmd)
{
	CDelay del(30);

    CLightPanelController* pController = (CLightPanelController*)app->m_Constructor.GetObject(eLight);
    int action = cmd.GetAction(); 

    if(cmd.IsCommand(BTN_AUTO_ALL))
        pController->m_btnAutoAll->m_button.setValue(action);
    else
    if(cmd.IsCommand(BTN_AUTO_CEIL))
        pController->m_btnAutoCeil->m_button.setValue(action);
    else
    if(cmd.IsCommand(BTN_AUTO_SING))
        pController->m_btnAutoSingle->m_button.setValue(action);
    else
    if(cmd.IsCommand(SHOW_MAIN_MENU))
        app->Show();
    else
    {
        switch (cmd.buff[2])
        {
        case 't':  // usual lamp
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
        case 's': // sens lamp
            {
                for(int i = 0 ; i < m_countBtnsSens; i+=2)
                {
                    if(cmd.IsName(m_btnsSens[i]->m_name))
                    {
                        m_btnsSens[i]->SetValue(action);
                        return;
                    }
                }
            }
        case 'a': // sens button
            {
                for(int i = 1 ; i < m_countBtnsSens; i+=2)
                {
                    if(cmd.IsName(m_btnsSens[i]->m_name))
                    {
                        m_btnsSens[i]->SetValue(action);
                        return;
                    }
                }
                break;
            }
        default:
            SMES("Unhandle command: ");
            SMESN(cmd.buff);
        }
    }
    

}

//------------------------------------------------------------------------



