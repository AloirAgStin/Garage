#include "ElecLightPanelController.h"
#include "MainAppl.h"

extern CMainAppl *app;

const short g_countSingleObject = 10;

CDoorPanelController::CDoorPanelController(uint8_t pageID, uint8_t objID, const char *pageName, CSendCmd * _SerialCmd) 
    : CNexPageEx(pageID,objID,pageName),
      m_SerialCmd(_SerialCmd)
{
    m_Btns   = 0;
    m_countBtns   = 0;
}

CDoorPanelController::~CDoorPanelController(void)
{
    delete_2x_Array(m_Btns, m_countBtns)
}

void CDoorPanelController::Init()
{
    m_currListenItem = 0;
    for(int i = 0 ; i < g_countNextListenItemDoor; i++)
        m_nex_listen_list[i] = 0;


    int otherButtonCmd = 1;
    m_btnMenu = new NexButton(m_pageID, 1, "bMenu");  
    m_btnMenu->attachPush(CDoorPanelController::EventPageButton, (void*)otherButtonCmd);
    m_nex_listen_list[m_currListenItem++] = m_btnMenu;
    

    m_Btns = new CNextionSingleObject*[g_countSingleObject];

    //street people gate
    AddObject(m_pageID, 21, "gk1", DR_LOCK_STREET_GATE);
    AddObject(m_pageID, 22, "gk0", DR_UNLOCK_STREET_GATE);
    
    //door to garage
    AddObject(m_pageID, 25, "gd1", DR_LOCK_GARAGE_DOOR);
    AddObject(m_pageID, 26, "gd0", DR_UNLOCK_GARAGE_DOOR);
    
    //main garage door
    AddObject(m_pageID, 17, "gg1", DR_GARAGE_GATE_OPEN);
    AddObject(m_pageID, 18, "gg0", DR_GARAGE_GATE_CLOSE);
    
    //garage luke
    AddObject(m_pageID, 24, "gl1", DR_OPEN_LUKE);
    AddObject(m_pageID, 23, "gl0", DR_CLOSE_LUKE);
    
    //street main gate
    AddObject(m_pageID, 19, "gs1", DR_STREET_GATE_OPEN);
    AddObject(m_pageID, 20, "gs0", DR_STREET_GATE_CLOSE);    

    
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


void CDoorPanelController::AddObject(uint8_t pid, uint8_t cid, const char* NameObj, const char* cmd)
{
    m_Btns[m_countBtns] = new CNextionSingleObject(pid, cid, NameObj, cmd);
    CNextionSingleObject* object = m_Btns[m_countBtns];

    object->GetNextObj()->attachPush(CDoorPanelController::EventPressButton, object);
    m_nex_listen_list[m_currListenItem++] = object->GetNextObj();

    m_countBtns++;
}

void CDoorPanelController::EventPressButton(void *ptr)
{    
    CNextionSingleObject *object = (CNextionSingleObject*)ptr;

    CDoorPanelController* pController = (CDoorPanelController*)app->m_Constructor.GetObject(eDoor);
    pController->m_SerialCmd->SendCommand(object->m_cmd);
}



void CDoorPanelController::Show()
{
    show();// show page
}


void CDoorPanelController::EventPageButton(void *ptr)
{
  int n = (int)ptr;

  if(n == 1)        //go to main menu
    app->Show();
}

void CDoorPanelController::ProcessCMD(sCmdWord &cmd)
{
    if(cmd.IsCommand(SHOW_MAIN_MENU))
       app->Show();
    else
    {
        for(short i = 0; i < m_countBtns; i++)
        {
            if(cmd.IsName(m_Btns[i]->m_name))
            {
                uint32_t color = cmd.GetAction() == 1 ? 63586 : 33792;
                m_Btns[i]->SetColor(color);
                return;
            }
        }

    }
}


//------------------------------------------------------------------------

