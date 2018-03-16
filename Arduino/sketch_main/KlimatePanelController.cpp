#include "KlimatePanelController.h"
#include "MainAppl.h"

extern CMainAppl *app;

const short g_countTxtObject = 2;

const short g_countDSObject = 4;
const short g_countSingleObject = 6;

CKlimatePanelController::CKlimatePanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd) 
    : CNexPageEx(pageID,objID,pageName),
      m_SerialCmd(_SerialCmd)
{
    m_Btns   = 0;
    m_countBtns   = 0;

    m_countDSBtns = 0;
    m_dsBtns = 0;

    m_txtObject = 0;
    m_countTxtObject = 0;
}

CKlimatePanelController::~CKlimatePanelController(void)
{
    delete_2x_Array(m_txtObject, m_countTxtObject)

    delete_2x_Array(m_Btns, m_countBtns)
    delete_2x_Array(m_dsBtns,  m_countDSBtns)
}

void CKlimatePanelController::Init()
{
    m_currListenItem = 0;
    for(int i = 0 ; i < g_countNextListenItemKlimate; i++)
        m_nex_listen_list[i] = 0;


    //dont change queue

    //create label controls 
    m_txtObject = new NexText*[g_countTxtObject];    

    //create label controls EXHAUSE
    NexText *obj = new NexText(m_pageID, 22, "tex");
    m_txtObject[m_countTxtObject++] = obj;
    
    //create label controls HEATING
    obj = new NexText(m_pageID, 17, "the");
    m_txtObject[m_countTxtObject++] = obj;


    //add usual buttons
    m_Btns = new CNextionSingleObject*[g_countSingleObject];
    
    AddObject(m_pageID, 13, "exa", KL_BTN_ADD_EXHAUST);
    AddObject(m_pageID, 14, "exm", KL_BTN_MIN_EXHAUST);

    AddObject(m_pageID, 15, "hea", KL_BTN_ADD_HEATING);
    AddObject(m_pageID, 16, "hem", KL_BTN_MIN_HEATING);

    AddObject(m_pageID, 10, "fna", KL_BTN_ADD_FAN);
    AddObject(m_pageID, 11, "fnm", KL_BTN_MIN_FAN);

    m_dsBtns = new CNextionDSObject*[g_countDSObject];
    
    //dont change queue
    AddObjectDS(m_pageID, 19, "exF", KL_BTN_EXHAUST_ONOFF, "");
    AddObjectDS(m_pageID, 20, "exA", KL_BTN_EXHAUST_AUTO,  "");
    AddObjectDS(m_pageID, 18, "heO", KL_BTN_HEATING_ONOFF, "");    
    AddObjectDS(m_pageID, 12, "fnA", KL_BTN_FAN_AUTO,      "");


    int otherButtonCmd = 1;
    m_btnMenu = new NexButton(m_pageID, 1, "bMenu");  
    m_btnMenu->attachPush(CKlimatePanelController::EventPageButton, (void*)otherButtonCmd);
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

void CKlimatePanelController::AddObjectDS(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_dsBtns[m_countDSBtns] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* object = m_dsBtns[m_countDSBtns];

    object->GetNextObj()->attachPush(CKlimatePanelController::EventPressDSButton, object);
    m_nex_listen_list[m_currListenItem++] = object->GetNextObj();

    m_countDSBtns++;
}

void CKlimatePanelController::EventPressDSButton(void *ptr)
{
    CNextionDSObject *object = (CNextionDSObject*)ptr;

    uint32_t value = 0;
    object->m_button.getValue(&value);
    object->m_oldValue = value;

    CKlimatePanelController* pController = (CKlimatePanelController*)app->m_Constructor.GetObject(eKlimate);
    pController->m_SerialCmd->SendCommand(object->GetCmdByValue(value));
}

void CKlimatePanelController::AddObject(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmd)
{
    m_Btns[m_countBtns] = new CNextionSingleObject(pid, cid, NameObj, cmd);
    CNextionSingleObject* object = m_Btns[m_countBtns];

    object->GetNextObj()->attachPush(CKlimatePanelController::EventPressButton, object);
    m_nex_listen_list[m_currListenItem++] = object->GetNextObj();

    m_countBtns++;
}

void CKlimatePanelController::EventPressButton(void *ptr)
{    
    CNextionSingleObject *lamp = (CNextionSingleObject*)ptr;

    CKlimatePanelController* pController = (CKlimatePanelController*)app->m_Constructor.GetObject(eKlimate);
    pController->m_SerialCmd->SendCommand(lamp->m_cmd);
}


void CKlimatePanelController::Show()
{
    show();// show page
}

void CKlimatePanelController::EventPageButton(void *ptr)
{
  int n = (int)ptr;

  if(n == 1)        //go to main menu
    app->Show();
}

void CKlimatePanelController::ProcessInvalidateCMD(sCmdInvalidate& cmd)
{

}


void CKlimatePanelController::ProcessCMD(sCmdWord &cmd)
{
	CDelay del(30);

    int action = cmd.GetAction();


    if(cmd.Type == sCmdWord::eMSGType::eCMD_FLOAT)
    {
        float val = 0.0;
        char buffText[7] = {0};
        cmd.GetFloatNumber(val,buffText);

        if(cmd.IsCommand(KL_TEXT_HEATING))
            m_txtObject[1]->setText(buffText);
    }
    else
    if(cmd.Type == sCmdWord::eMSGType::eCMD_NUMBER5)
    {
        char buffText[7] = {0};
        cmd.GetNumber(buffText);
        if(cmd.IsCommand(KL_TEXT_EXHAUST))
            m_txtObject[0]->setText(buffText);
    }
    else
    if(cmd.IsCommand(SHOW_MAIN_MENU))
        app->Show();
    else
    {
        for(int i = 0; i < m_countDSBtns; i++)
        {
            if(cmd.IsName(m_dsBtns[i]->m_name))
            {
                m_dsBtns[i]->SetValue(action);
                return;
            }
        }
    }
}


//------------------------------------------------------------------------

