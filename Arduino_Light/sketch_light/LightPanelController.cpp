#include "LightPanelController.h"

#include "LightItems.h"
#include "SerialController.h"

#define CMD_DELAY 50 //delay between command (this page)


short CLightPanelController::SetupObjects()
{
    //timer tick on for lamp sensor
    unsigned long time = 3; // second
    
#ifndef WIN32 
    time*= 1000L; //milisecond
#endif

    //need setup PIN LIGHT, it's first value
    //need setup PIN SENSOR, it's second value
    
    //slots for door and gate sensor lamp
    m_slots[0] = AddSensorLight(34, 42, time, "bsG", AUTO_BULB_LIGHT_1, "", "baG", AUTO_BULB_LIGHT_1_SENS, "");
    m_UlicaLampID1 = m_arrLightSensSwitches[0].GetID();

    m_slots[1] = AddSensorLight(35, 42, time, "bsD", AUTO_BULB_LIGHT_2, "", "saD", AUTO_BULB_LIGHT_2_SENS, "");      
	m_UlicaLampID2 = m_arrLightSensSwitches[1].GetID();

      //set PIN to microwave sensor
    AddMicroWaveSensor(7);  
        
    //add sensor lamp    
	//todo поменять сенсоры
    m_slots[2] = AddSensorLight(22, 36, time, "bs1", AUTO_BULB_LIGHT_3, "", "sa1", AUTO_BULB_LIGHT_3_SENS, "");
    m_slots[3] = AddSensorLight(23, 0, time, "bs2", AUTO_BULB_LIGHT_4, "", "sa2", AUTO_BULB_LIGHT_4_SENS, "");
  
    //AddConvertor(10);
    m_slots[4] = AddSensorLight(24, 38, time, "bs3", AUTO_BULB_LIGHT_5, "", "sa3", AUTO_BULB_LIGHT_5_SENS, "");
    m_slots[5] = AddSensorLight(25, 39, time, "bs4", AUTO_BULB_LIGHT_6, "", "sa4", AUTO_BULB_LIGHT_6_SENS, "");
        
    //AddConvertor(10);
    m_slots[6] =  AddSensorLight(26, 40, time, "bs5", AUTO_BULB_LIGHT_7, "", "sa5", AUTO_BULB_LIGHT_7_SENS, "");
    m_slots[7] =  AddSensorLight(27, 41, time, "bs6", AUTO_BULB_LIGHT_8, "", "sa6", AUTO_BULB_LIGHT_8_SENS, "");
    

    //add usual lamps
    m_slots[8]  = AddLight(28, 0,  "bt1", BULB_LIGHT9, "");
    m_slots[9]  = AddLight(29, 0,  "bt2", BULB_LIGHT10, "");
    
    m_slots[10] = AddLight(30, 0, "bt3", BULB_LIGHT11,  "");    
    m_slots[11] = AddLight(31, 0, "bt4", BULB_LIGHT12,  "");
    
    m_slots[12] = AddLight(32, 0, "bt5", BULB_LIGHT13,  "");
    

    short PIN_COOLER = 11;
    m_cooler = new CInverseDigitalOnOff(PIN_COOLER);
    
    return 1;
}

void CLightPanelController::SetAutoALL(bool IsOn)
{
    if(IsOn)
    {
		TurnOFFAutoBtn(BTN_SINGLE);
        TurnOFFAutoBtn(BTN_CEIL);
		InvalidateB();
	}
    
    m_isAutoAll = IsOn;
    
    SMES("Set AutoALL = ");
    SMESN(m_isAutoAll);


	InvalidateA(); //invalidate send lamp
    if(m_isAutoAll == false)
    {
        //usual lamp off
        for(int i = 0; i < m_CountLight; i ++)
            m_arrLightSwitches[i].Off();
	//	InvalidateB();
    }
		

	InvalidateB();
	ShowAutoStatus();
}

void CLightPanelController::SetAutoCeiling(bool IsOn)
{
    m_IsAutoCeiling = IsOn;
    SMES("Set AutoCeil = ");
    SMESN(m_IsAutoCeiling );
	
    if(IsOn)
    { 
		TurnOFFAutoBtn(BTN_SINGLE);
		TurnOFFAutoBtn(BTN_ALL);

        //usual lamp off
        for(int i = 0; i < m_CountLight; i ++)
        {
            short action = -1;
            m_arrLightSwitches[i].Off();
         }
		InvalidateB();
    }
    
    if(!IsOn)
        return;

    for(int i = 0; i < m_CountSensLight; i++)
    {
        if(m_UlicaLampID1 == m_arrLightSensSwitches[i].GetID() || m_UlicaLampID2 == m_arrLightSensSwitches[i].GetID() )
            continue;

        if(m_arrLightSensSwitches[i].GetPinStatus() == eStatusOnOFF::ST_ON)
            m_arrLightSensSwitches[i].On();
        else
            m_arrLightSensSwitches[i].Off();
    }    

    InvalidateA();

    TurnOFFAutoBtn(BTN_ALL);
    TurnOFFAutoBtn(BTN_SINGLE);

	InvalidateB();
    ShowAutoStatus();
}

void CLightPanelController::SetAutoSingle(bool IsOn)
{
    if(IsOn)
    {
        TurnOFFAutoBtn(BTN_ALL);
        TurnOFFAutoBtn(BTN_CEIL);
    
		//usual lamp off
        for(int i = 0; i < m_CountLight; i ++)
        	m_arrLightSwitches[i].Off();
        InvalidateB();
    }

    SMES("Set AutoSingl = ");
    SMESN(m_isAutoSingle);
    m_isAutoSingle = IsOn;
    
    for(int i = 0; i < m_CountSensLight; i++)
    {
        if(m_UlicaLampID1 == m_arrLightSensSwitches[i].GetID() || m_UlicaLampID2 == m_arrLightSensSwitches[i].GetID() )
            continue;

        if(m_isAutoSingle == true)
            m_arrLightSensSwitches[i].SetAuto();
        else
        	m_arrLightSensSwitches[i].Off();
    }    
	InvalidateA();
	InvalidateB();
	/*
	{
		sCmdWord cmdAutoSingl(BTN_AUTO_SING);
		cmdAutoSingl.SetAction(m_isAutoSingle == true ? 1 : 0);
		SendCMD(&cmdAutoSingl);      
	}
	*/
	
   ShowAutoStatus();
}

int CLightPanelController::Process()
{   
    
    m_MicroWaveSensor->process();

    // "Auto All" - all sensor lamp and usual lamp on/off by Microwave sensor (only) 
    if(m_isAutoAll || m_IsAutoCeiling)
    {
        //sens button
        for(int i = 0; i < m_CountSensLight; i++)
        {
            //except lamp on door and gate
            if(m_UlicaLampID1 == m_arrLightSensSwitches[i].GetID() || m_UlicaLampID2 == m_arrLightSensSwitches[i].GetID() )
                continue;

            short action = -1;
            if(m_MicroWaveSensor->m_IsAllowlight)
            {
                if(m_arrLightSensSwitches[i].On())
                {
                    action = 1;
               //todo      delay(CMD_DELAY);
                }
            }
            else
            {
                if(m_arrLightSensSwitches[i].Off())
                {
                    action = 0;
                //todo     delay(CMD_DELAY);
                }
            }

            if(action != -1)
                SendCMD(m_arrLightSensSwitches[i].m_btnBulb.GetCmdByValue(action));
        }                        
        
        //usual lampn
        if(m_isAutoAll)
        {
            for(int i = 0; i < m_CountLight; i ++)
            {
                short action = -1;
                if(m_MicroWaveSensor->m_IsAllowlight)
                {
                    if(m_arrLightSwitches[i].On())
                    {
                        action = 1;
                //todo         delay(CMD_DELAY);
                    }
                }
                else
                {
                    if(m_arrLightSwitches[i].Off())
                    {
                        action = 0;
                      //todo   delay(CMD_DELAY);
                    }
                }

                if(action != -1)
                {
                  SendCMD(m_arrLightSwitches[i].GetCmdForInvalidate());
                  delay(CMD_DELAY);                  
                }
            }
        }
        
    }
    else
    {
        //if not auto all
        CLightSwitchSensor::eIllumination il = m_MicroWaveSensor->m_IsAllowlight ? CLightSwitchSensor::DARK : CLightSwitchSensor::LIGHT;
             bool IsBULBActive = IsRoomBulbActive(); // is active any bulb

        for(int i = 0; i < m_CountSensLight; i++)
        {
            //except lamp on door and gate
            if(m_UlicaLampID1 == m_arrLightSensSwitches[i].GetID() || 
                m_UlicaLampID2 == m_arrLightSensSwitches[i].GetID() )
                continue;
            m_arrLightSensSwitches[i].m_IllStatus = il;
        

            if(!IsBULBActive)
                m_arrLightSensSwitches[i].ForceSensorSignal();
        }
    }

    //if signal isn't exists, trun off all usual lamp
    if(false == m_MicroWaveSensor->m_IsAllowlight && !m_isAutoAll)
    {
		const int time_Delay = 
			#ifdef WIN32
						1;
			#else
						1000;
			#endif		

        static unsigned long lastdraw = 0;
        if(millis() - lastdraw > time_Delay)
        {
			//todo check
			SMESN("TRY TO OFF ALL SENSOR");

            for(int i = 0; i < m_CountLight; i++)
            {
                if(m_arrLightSwitches[i].Off())
                {
                  SendCMD(m_arrLightSwitches[i].m_nextionButton.GetCmdByValue(0));
                  delay(CMD_DELAY);                  
                }
            }

            lastdraw = millis();
        }
    }

    for(int i = 0; i < m_CountSensLight; i++)
        m_arrLightSensSwitches[i].Update();

    if(m_cooler)
    {
        if(IsAnyBulbActive())
        {
           if(m_cooler->On())
              SMESN("COOLER ON");
        }
        else
        {
          if(m_cooler->Off())
            SMESN("COOLER OFF");
        }
    }

    return 1;
}

void CLightPanelController::SendCMD(sCmdWord* cmd)
{
    //if active page not 1, return
    if(CSerialController::ActivepaPage != eLight)
        return;

    cmdArduino->SendCommand(cmd);
}

void CLightPanelController::ProcessSerialCMD(sCmdWord &cmd)
{
    short action = cmd.GetAction();

    if(cmd.IsCommand(SHOW_LIGHT_PAGE))
        InvalidateScreen();
    else    
    if(cmd.IsCommand(BTN_AUTO_ALL))
        SetAutoALL(action == 1 ? true : false);
    else
    if(cmd.IsCommand(BTN_AUTO_CEIL))
        SetAutoCeiling(action == 1 ? true : false);
    else
    if(cmd.IsCommand(BTN_AUTO_SING))
        SetAutoSingle(action == 1 ? true : false);
    else
    {
        switch (cmd.buff[2])
        {
            case 't':  //then push any lamp button
            {
                for(int i = 0; i < m_CountLight; i++)
                {
                    if(cmd.IsName(m_arrLightSwitches[i].m_nextionButton.m_Name))
                    {
                        SMES("FIND USUAL "); SMESN(action);
                        m_arrLightSwitches[i].OnOFF(action);        
                        break;
                    }            
                }
                break;
            }
            case 'a' : // sens lamp button (auto)
                {
                    for(int i = 0 ; i < m_CountSensLight; i++)
                    {
                        if(cmd.IsName(m_arrLightSensSwitches[i].m_btnSensors.m_Name))
                        {
                            SMES("FIND SENS(AUTO) "); SMESN(action);
                            if(action == 1)
                            {
                                m_arrLightSensSwitches[i].SetAuto();
                            }
                            else
                            {
                                m_arrLightSensSwitches[i].Off();
                            }

                            break;
                        }
                    }
                    break;
                }
            case 's': // sens lamp 
                {
                    //looking sensor
                    for(int i = 0; i < m_CountSensLight; i++)
                    {
                        if(cmd.IsName(m_arrLightSensSwitches[i].m_btnBulb.m_Name))
                        {
                            SMES("FIND SENS SWITCH "); SMESN(action);
                            m_arrLightSensSwitches[i].OnOFF(action);
                            break;
                        }
                    }

                    break;
                }
        }
    }

    return;
}

void CLightPanelController::Show()
{
    InvalidateScreen();
}

void CLightPanelController::InvalidateA()
{
	sCmdInvalidate invA(CMD_INVALIDATE_A);
	for(int i = 0; i < m_CountSensLight; i++)
	{
		invA.AddNextSymb(m_arrLightSensSwitches[i].GetCmdForInvalidateBtn()->GetAction() == 1 ? '1' : '0');
		invA.AddNextSymb(m_arrLightSensSwitches[i].GetCmdForInvalidateSens()->GetAction() == 1 ? '1' : '0');
	}
	cmdArduino->SendCommand(invA.buff);
	cmdArduino->flush();
}

void CLightPanelController::InvalidateB()
{
	sCmdInvalidate invB(CMD_INVALIDATE_B);
	
	for(int i = 0; i < m_CountLight; i++)
		invB.AddNextSymb(m_arrLightSwitches[i].GetCmdForInvalidate()->GetAction() == 1 ? '1' : '0');
	invB.AddNextSymb(m_isAutoAll == true ? '1' : '0');
	invB.AddNextSymb(m_IsAutoCeiling == true ? '1' : '0');
	invB.AddNextSymb(m_isAutoSingle == true ?  '1' : '0');
	cmdArduino->SendCommand(invB.buff);
	cmdArduino->flush();
}

void CLightPanelController::InvalidateScreen()
{
    SMESN("Inv LIGHT");
    int inDelay = 100;
    
    ShowAutoStatus();

	InvalidateA();
	InvalidateB();


	return;

    {
       sCmdWord cmdAutoAll(BTN_AUTO_ALL);
       cmdAutoAll.SetAction(m_isAutoAll == true ? 1 : 0);
       SendCMD(&cmdAutoAll);
    }
    
    {
        sCmdWord cmdAutoCeil(BTN_AUTO_CEIL);
        cmdAutoCeil.SetAction(m_IsAutoCeiling == true ? 1 : 0);
        SendCMD(&cmdAutoCeil);
        delay(inDelay);
    }

    {
      sCmdWord cmdAutoSingl(BTN_AUTO_SING);
      cmdAutoSingl.SetAction(m_isAutoSingle == true ? 1 : 0);
      SendCMD(&cmdAutoSingl);      
    }

    //read all item and draw their status
    for(int i = 0; i < m_CountLight; i++)
    {
      SendCMD(m_arrLightSwitches[i].GetCmdForInvalidate());
      delay(inDelay);
    }

    SMESN("-------------------------------------");
    for(int i = 0; i < m_CountSensLight; i++)
    {
        SendCMD(m_arrLightSensSwitches[i].GetCmdForInvalidateBtn());
        delay(inDelay);
        
        SendCMD(m_arrLightSensSwitches[i].GetCmdForInvalidateSens());
        delay(inDelay);
    }
}

bool CLightPanelController::IsActiveAnyMotionSens()
{
    for(int i = 0; i < m_CountSensLight; i++)
    {
        if(m_UlicaLampID1 == m_arrLightSensSwitches[i].GetID() || 
            m_UlicaLampID2 == m_arrLightSensSwitches[i].GetID() )
            continue;

        if(m_arrLightSensSwitches[i].IsMotionDeteted())
            return true; 
    }
    return false;
}

bool CLightPanelController::IsRoomBulbActive()
{
    for(int i = 0; i < m_CountSensLight; i++)
    {
        if(m_UlicaLampID1 == m_arrLightSensSwitches[i].GetID() || 
            m_UlicaLampID2 == m_arrLightSensSwitches[i].GetID() )
            continue;

        if(m_arrLightSensSwitches[i].GetPinStatus() == eStatusOnOFF::ST_ON)
            return true;
    }
    return false;
}

bool CLightPanelController::IsAnyBulbActive()
{
    for(int i = 0; i < m_CountSensLight; i++)
    {
        if(m_arrLightSensSwitches[i].GetPinStatus() == eStatusOnOFF::ST_ON)
            return true;
    }

    for(int i = 0; i < m_CountLight; i++)
    {
        if(m_arrLightSwitches[i].GetObjectPinStatus() == eStatusOnOFF::ST_ON)
            return true;
    }

    return false;
}

int CLightPanelController::AddLight(short PIN_Light, short PIN_CONVERTOR, const char* NameObj, const char *cmdOn, const char *cmdOff)
{
    if(m_CountLight + 1 > COUNT_USUAL_LAMP)
        return 0;

    m_arrLightSwitches[m_CountLight] = CLightSwitch(PIN_Light, NameObj,cmdOn, cmdOff);
    m_CountLight++;
    return m_arrLightSwitches[m_CountLight].GetID();
}

//------------------------------------------------------------------------

int CLightPanelController::AddSensorLight(short PIN_Light, short PIN_Sensor, unsigned int time,
    const char* lampName, const char* LampcmdOn, const char* LampcmdOff,
    const char* sensName, const char* SenscmdOn, const char* SenscmdOff)
{
    if(m_CountSensLight + 1 > COUNT_SENS_LAMP)
        return 0;
    m_arrLightSensSwitches[m_CountSensLight] = CLightSwitchSensor(PIN_Light,PIN_Sensor, time, lampName, LampcmdOn,LampcmdOff, sensName, SenscmdOn, SenscmdOff);
    m_arrLightSensSwitches[m_CountSensLight].m_timer->SetParent(&m_arrLightSensSwitches[m_CountSensLight]);

    m_CountSensLight++;
    return m_arrLightSensSwitches[m_CountSensLight].GetID();
}


int CLightPanelController::AddMicroWaveSensor(short PIN_Sensor)
{
    if(m_MicroWaveSensor)
        delete m_MicroWaveSensor;

    m_MicroWaveSensor = new CMicroWaveSensor(PIN_Sensor);
    return m_MicroWaveSensor->GetID();
}


bool CLightPanelController::SetOnOffBulb(int ID, bool IsOn)
{    
  for(int i = 0; i < m_CountSensLight; i++)
    {
        if(m_arrLightSensSwitches[i].GetID() == ID)
        {
            m_arrLightSensSwitches[i].OnOFF(IsOn == true ? 1 : 0);
            return true;
        }
    }
 
    for(int i = 0; i < m_CountLight; i++)
    {
        if(m_arrLightSwitches[i].GetID() == ID)
        {
            m_arrLightSwitches[i].OnOFF(IsOn == true ? 1 : 0);
            return true;
        }       
    }
    return false;
}


CLightPanelController::CLightPanelController(CSendCmd *port) 
{
    m_CountSensLight    = 0;
    m_CountLight        = 0;
    m_MicroWaveSensor   = 0;
    m_cooler            = 0;
    cmdArduino          = port;

    for(int i = 0; i < g_countSlotsLight; i++)
        m_slots[i] = 0;
}


short CLightPanelController::Init()
{
    m_UlicaLampID1 = 0;
    m_UlicaLampID2 = 0;

    m_isAutoSingle        = 0;
    m_IsAutoCeiling        = 0;
    m_isAutoAll            = 0;

    short ret = 0;
    ret = SetupObjects();

    if(ret != 1) return ret;

    for(int i = 0; i < m_CountSensLight; i++)
        m_arrLightSensSwitches[i].Off();         

    for(int i = 0; i < m_CountLight; i++)
        m_arrLightSwitches[i].Off();   

    return 1;
}

CLightPanelController::~CLightPanelController(void)
{
    delete m_cooler;
    delete m_MicroWaveSensor;
}



void CLightPanelController::TurnOFFAutoBtn(AUTO_BTN n)
{
    //turn off AutoALL
    if(n == BTN_ALL)
    {
        if(m_isAutoAll)
        {
            m_isAutoAll = 0;
			/*
            sCmdWord cmd(BTN_AUTO_ALL);
            cmd.SetAction(0);
            SendCMD(&cmd);
            delay(CMD_DELAY);*/
        }
    }
    else
    if(n == BTN_CEIL)
    {
        //turn off Ceiling
        if(m_IsAutoCeiling) 
        {
            m_IsAutoCeiling = 0;
			/*
            sCmdWord cmd(BTN_AUTO_CEIL);
            cmd.SetAction(0);
		    SendCMD(&cmd);
			delay(CMD_DELAY);*/
        }
    }
    else
    if(n == BTN_SINGLE)
    {
        //turn off Single
        if(m_isAutoSingle) 
        {
            m_isAutoSingle = 0;
			/*
            sCmdWord cmd(BTN_AUTO_SING);
            cmd.SetAction(0);
            SendCMD(&cmd);
            delay(CMD_DELAY);*/
        }
    }
}

void CLightPanelController::ShowAutoStatus()
{
    SMES("AutoAll: ");
    SMESN(m_isAutoAll);
    
    SMES("AutoCeiling: ");
    SMESN(m_IsAutoCeiling);
    
    SMES("AutoSingle: ");
    SMESN(m_isAutoSingle);    
}

