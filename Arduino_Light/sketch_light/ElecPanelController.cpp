#include "ElecPanelController.h"

#include "SerialController.h"

extern CLightPanelController*    m_panelLight;

const short CElecPanelController::m_maxSocetCount = 8 * 2;
int CSimpleSocet::OffDelay = 100;

short CElecPanelController::SetupObjects()
{
    short nextSlot = 0; //NOT MODIFY

    //set network voltage PIN sensor
    short pinVoltage = A3;
    m_VoltageReader = new CDigitalSensor(pinVoltage);

    //set delay after off every socket
    CSimpleSocet::OffDelay = 100; 

    //time to OFF socket by timer
    unsigned  long timeOff = 10;
#ifndef WIN32
    timeOff *= 5000L; //milisecond
#endif

    //need setup PIN, it's first value for each item

    //SETUP SOCKET -- HIGH SIGNAL
    short PIN_SOCKET = 51;
    m_arrSocketSwitches[m_countSocket++] = new CSimpleSocet(PIN_SOCKET, timeOff, "bt1", EL_POINT_1, "", "bf1", EL_POINT_PROP_1, "");
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID(); //SOCKET 1
    
    
    //SETUP MULTIMEDIA SYSTEM //HIGH SIGNAL
    short PIN_IndicatorMediaSystem = 48; //set pin indicator
    short PIN_MULTIMEDIA = 49;

    m_mediaSystem = new CMultimediaSystem(PIN_MULTIMEDIA, timeOff, "bt2", EL_POINT_2, "", "bf2", EL_POINT_PROP_2, "", PIN_IndicatorMediaSystem);
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_arrSocketSwitches[m_countSocket++] = m_mediaSystem;
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID(); //MULTIMEDIA 2
    

    //SETUP SCREEN
    short PIN_DISPLAY = 43; //LOWSIGNAL
    m_arrSocketSwitches[m_countSocket++] = new CDisplaySocket(PIN_DISPLAY, timeOff, "bt3", EL_POINT_3, "", "bf3", EL_POINT_PROP_3, "");
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID(); //SCREEN 3

    //SETUP CHARGE BOX
    short PIN_CHARGE_BOX = 52;   //HIGH SIGNAL
    m_arrSocketSwitches[m_countSocket++] = new CSimpleSocet(PIN_CHARGE_BOX, timeOff, "bt4", EL_POINT_4, "", "bf4", EL_POINT_PROP_4, "");
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID(); //CHARGE BOX 4
    
    //SETUP UPS 1
    short PIN_UPS1 = 46;      //HIGH SIGNAL
    short PIN_INDICATOR_UPS_1 = 47;
    m_arrSocketSwitches[m_countSocket++] = new CUPSSocket(PIN_UPS1, timeOff, "bt5", EL_POINT_5, "", "", "", "", PIN_INDICATOR_UPS_1);
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID(); //UPS 1

    short PIN_UPS2 = 44;
    short PIN_INDICATOR_UPS_2 = 45;
    m_arrSocketSwitches[m_countSocket++] = new CUPSSocket(PIN_UPS2, timeOff, "bt6", EL_POINT_6, "", "", "", "", PIN_INDICATOR_UPS_2);
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID(); //UPS 1
        

    //SETUP BUTTON BEETWEN UPS 1 AND UPS 2
    short PIN_BUTTON_UPS = 53;
    m_arrSocketSwitches[m_countSocket++] = new CSimpleSocet(PIN_BUTTON_UPS, timeOff, "bt7", EL_POINT_7_AUTOM, "", "", "", "");
    m_arrSocketSwitches[m_countSocket - 1]->SetTimerParent(m_arrSocketSwitches[m_countSocket-1]);
    m_slots[nextSlot++] = m_arrSocketSwitches[m_countSocket - 1]->GetID();

    //SETUP COOLER PIN
    short PIN_COOLER = 13;
    m_cooler = new CInverseDigitalOnOff(PIN_COOLER);
    
    return 1;
}

void CElecPanelController::SendCMD(sCmdWord* cmd)
{
    //if active page not 4, return
    if(CSerialController::ActivepaPage != eElectNetWork)
        return;
        
    cmdArduino->SendCommand(cmd);
}


void CElecPanelController::ProcessSerialCMD(sCmdWord &cmd)
{
  if(cmd.IsCommand(SHOW_ELECT_NET_PAGE))
     InvalidateScreen();
  else
  if(cmd.buff[1] == 'b')
  {
        int action = cmd.GetAction();

        //bs - btn
        //bf - property
        if( cmd.buff[2] == 's' || cmd.buff[2] == 'f' )
        {
                 int num = cmd.buff[3] - '0' - 1;
                 if(num < m_countSocket)
                 {
                      CSimpleSocet *item = m_arrSocketSwitches[num];
                      if(!item) return;
      
                      bool IsOk = false;
                      if(cmd.buff[2] == 's')
                      {
                          if(action == 1)
                              IsOk = item->OnSocet();
                          else
                              IsOk = item->OffSocet();
                      
                          SendCMD(item->GetCmdForInvalidateBtn());
                      }
                      else
                      {
                          if(num < 4)
                             item->m_PropertyOnOFF = action == 1 ? ST_ON: ST_OFF;                          
                      }
                 }
        }
     }   
}

int CElecPanelController::Process()
{   
    static bool lastVoltage = false;
    
    m_mediaSystem->UpdateIndicator();


    int val = m_VoltageReader->ReadData();
    if(val == ON)
        lastVoltage  = false;
    else 
    lastVoltage = true;

    //todo delete after instal sensor
    lastVoltage = true;
    
    for(int i = 0; i < m_countSocket; i++)
    {
        CSimpleSocet *item = m_arrSocketSwitches[i]; 
        item->SetNetworkVoltage(lastVoltage);

        if(item->m_PropertyOnOFF == ST_ON) // if work in auto mode
        {
            if(m_panelLight->IsActiveAnyMotionSens())
            {
                bool IsOk = item->OnSocet();     //immediately turn ON socket
                item->StartStopKillTimer(false); //and kill timer
                if(IsOk)
                   SendCMD(item->GetCmdForInvalidateBtn());
                 //todo delay(50);
            }
            else
                item->StartStopKillTimer(true); //set timer for kill socket
        }
        
        item->Update();

        if(item->IsNeedOFF())
        {
            item->OffSocet();
            SendCMD(item->GetCmdForInvalidateBtn());
           //todo  delay(50);
        }

        //if any socket on => need on cooler
        if(item->GetStatus() == ST_ON && m_cooler)
            m_cooler->On();
    }
    
    return 1;
}

bool CElecPanelController::IsAnySocketON()
{ 
    for(int i = 0; i < m_countSocket; i++)
    {
        CSimpleSocet *item = m_arrSocketSwitches[i]; 
        
        //if any socket on => need on cooler
        if(item->GetStatus() == ST_ON)
            return true;
    }

    return false;
}

void CElecPanelController::TurnOFFCooler()
{
    if(m_cooler)
        m_cooler->Off();
}


void CElecPanelController::Show()
{
    InvalidateScreen();
}

void CElecPanelController::InvalidateScreen()
{
     SMESN("Inv Elect");

	  sCmdInvalidate invA(CMD_EL_INVALIDATE_A);
	 
	  for(int i = 0; i < m_countSocket; i++)
	 {
		 invA.AddNextSymb(m_arrSocketSwitches[i]->GetCmdForInvalidateBtn()->GetAction() == 1 ? '1' : '0');
		 if(i < 4)
			 invA.AddNextSymb(m_arrSocketSwitches[i]->GetCmdForInvalidateBtn()->GetAction() == 1 ? '1' : '0');
	 }
	 cmdArduino->SendCommand(invA.buff);
	 cmdArduino->flush();


    //read all item and draw their status 
	 //todo del
     /*for(int i = 0; i < m_countSocket; i++)
     {
        SendCMD(m_arrSocketSwitches[i]->GetCmdForInvalidateBtn());
        delay(50);

        if(i < 4)
        {
			SendCMD(m_arrSocketSwitches[i]->GetCmdForInvalidateProp());
			delay(50);
		}
    }  
	*/ 
}

CElecPanelController::CElecPanelController(CSendCmd *port) 
{
    m_mediaSystem   = 0;
    cmdArduino      = port;
    m_cooler        = 0;
    
    for(int i = 0; i < g_countSlots; i++)
        m_slots[i] = 0;

    m_arrSocketSwitches = new CSimpleSocet*[COUNT_SOCKET];
    m_countSocket = 0;
}

short CElecPanelController::Init()
{
    short ret = 0;
    ret = SetupObjects();

    if(ret != 1) return ret;   

    for(int i = 0; i < m_countSocket; i++)
        m_arrSocketSwitches[i]->OnSocet();  

    if(m_cooler)
        m_cooler->On();

    return 1;
}



CElecPanelController::~CElecPanelController(void)
{    
    delete_2x_Array(m_arrSocketSwitches, m_countSocket);
}
