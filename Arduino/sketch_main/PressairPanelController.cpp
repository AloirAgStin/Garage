#include "PressairPanelController.h"
#include "MainAppl.h"

extern CMainAppl *app;

const short g_countTxtObject = 3;

const short g_countDSObject = 2;
const short g_countSingleObject = 5;

CPressairPanelController::CPressairPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd) 
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

CPressairPanelController::~CPressairPanelController(void)
{
    delete_2x_Array(m_txtObject, m_countTxtObject)

    delete_2x_Array(m_Btns, m_countBtns)
    delete_2x_Array(m_dsBtns,  m_countDSBtns)
}

void CPressairPanelController::Init()
{
    //create labal controls
    m_txtObject = new NexText*[g_countTxtObject];    

    //Add current press
    NexText *obj = new NexText(m_pageID, 13, "tcp");
    m_txtObject[m_countTxtObject++] = obj;
    
    //Add max press
    obj = new NexText(m_pageID, 14, "tmp");
    m_txtObject[m_countTxtObject++] = obj;
    
    for(int i = 0 ; i < g_countNextListenItemPressair; i++)
        m_nex_listen_list[i] = 0;

    m_currListenItem = 0;

    //create one-click controls
    int otherButtonCmd = 1;
    m_btnMenu = new NexButton(m_pageID, 1, "bMenu");  
    m_btnMenu->attachPush(CPressairPanelController::EventPageButton, (void*)otherButtonCmd);
    m_nex_listen_list[m_currListenItem++] = m_btnMenu;

    //add usual buttons
    m_Btns = new CNextionSingleObject*[g_countSingleObject];
    
    AddObject(m_pageID, 15, "mpa", PA_BTN_MP_ADD);
    AddObject(m_pageID, 16, "mpm", PA_BTN_MP_MINS);

    AddObject(m_pageID, 12, "kds", PA_BTN_SINK);    

    m_dsBtns = new CNextionDSObject*[g_countDSObject];
    AddObjectDS(m_pageID, 10, "prs", PA_BTN_COMPRESSOR, "");
    AddObjectDS(m_pageID, 11, "kda", PA_BTN_CONDAUTO,   "");
    
    m_pDate    = new NexText(m_pageID, 5,  "tDate");
    m_pTime    = new NexText(m_pageID, 4,  "tTime");
    m_pTempIn  = new NexText(m_pageID, 3,  "tTpO");
    m_pTempOut = new NexText(m_pageID, 2,  "tTpI");
    m_pPress   = new NexText(m_pageID, 8, "tPress");
    m_pPower   = new NexText(m_pageID, 7, "tPower");
    m_pVoltage = new NexText(m_pageID, 6,  "tVolt");
    m_pAmpers  = new NexText(m_pageID, 9, "tAmper");
}


void CPressairPanelController::AddObjectDS(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmdON, const char* cmdOFF)
{
    m_dsBtns[m_countDSBtns] = new CNextionDSObject(pid, cid, NameObj, cmdON, cmdOFF);
    CNextionDSObject* object = m_dsBtns[m_countDSBtns];

    object->GetNextObj()->attachPush(CPressairPanelController::EventPressDSButton, object);
    m_nex_listen_list[m_currListenItem++] = object->GetNextObj();

    m_countDSBtns++;
}

void CPressairPanelController::EventPressDSButton(void *ptr)
{
    CNextionDSObject *object = (CNextionDSObject*)ptr;

    uint32_t value = 0;
    object->m_button.getValue(&value);
    object->m_oldValue = value;

    CPressairPanelController* pController = (CPressairPanelController*)app->m_Constructor.GetObject(ePressair);
    pController->m_SerialCmd->SendCommand(object->GetCmdByValue(value));
}


void CPressairPanelController::AddObject(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmd)
{
    m_Btns[m_countBtns] = new CNextionSingleObject(pid, cid, NameObj, cmd);
    CNextionSingleObject* object = m_Btns[m_countBtns];

    object->GetNextObj()->attachPush(CPressairPanelController::EventPressButton, object);
    m_nex_listen_list[m_currListenItem++] = object->GetNextObj();

    m_countBtns++;
}

void CPressairPanelController::EventPressButton(void *ptr)
{    
    CNextionSingleObject *lamp = (CNextionSingleObject*)ptr;

    CPressairPanelController* pController = (CPressairPanelController*)app->m_Constructor.GetObject(ePressair);
    pController->m_SerialCmd->SendCommand(lamp->m_cmd);
}



void CPressairPanelController::Show()
{
    show();// show page
}


void CPressairPanelController::EventPageButton(void *ptr)
{
  int n = (int)ptr;

  if(n == 1)        //go to main menu
    app->Show();
}

void CPressairPanelController::ProcessInvalidateCMD(sCmdInvalidate& cmd)
{

}

void CPressairPanelController::ProcessCMD(sCmdWord &cmd)
{
	CDelay del(30);

    int action = cmd.GetAction();

    if(cmd.Type == sCmdWord::eMSGType::eCMD_FLOAT)
    {
        float val = 0.0;
        char buffText[7] = {0};
        cmd.GetFloatNumber(val, buffText);
        
        if(cmd.IsCommand(PA_TEXT_PRESS_CUR))
            m_txtObject[0]->setText(buffText);
        if(cmd.IsCommand(PA_TEXT_PRESS_MAX))
            m_txtObject[1]->setText(buffText);
        if(cmd.IsCommand(PA_TEXT_PRESS_OUT))
            m_txtObject[2]->setText(buffText);
    }
    else
    if(cmd.IsCommand(PA_BTN_COMPRESSOR))
        m_dsBtns[0]->SetValue(action);
    else
    if(cmd.IsCommand(PA_BTN_CONDAUTO))
        m_dsBtns[1]->SetValue(action);
    else
    if(cmd.IsCommand(SHOW_MAIN_MENU))
        app->Show();
    else
    {
        SMES("Unhandle command: ");
        SMESN(cmd.buff);
    }
}

//------------------------------------------------------------------------

