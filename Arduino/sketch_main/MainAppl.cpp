#ifdef WIN32
#include "..\StdAfx.h"
#endif

#include "MainAppl.h"
#include "GlobalDef.h"

//setup 
#define SENSOR_TEMP_IN_OUT 3

#define SENSOR_PRESS     A3
#define SENSOR_POWER     A4
#define SENSOR_VOLT_AC   A0
#define SENSOR_VOLT_DC   A1

CMainAppl::CMainAppl(const char* namePage)
  : CNexPageEx(0, 0, namePage),
    cmdArduino(CSendCmd::eSerial2, 
                #ifdef DEBUG_INFO 
                    true 
                #else
                    false
                #endif 
                    ),
    cmdArduino2(CSendCmd::eSerial3, 
                #ifdef DEBUG_INFO 
                    true 
                #else
                    false
                #endif 
                    ),
    m_Constructor(&cmdArduino, &cmdArduino2),
      m_DateTime(RTC_DS3231)
{
    m_pageID = eMAIN;
    ActivePage = eMAIN;

    strcpy(m_date,    "00/00/00");
    strcpy(m_timer,   "00:00");
    strcpy(m_TempIn,  "0");
    strcpy(m_TempOut, "0");
    strcpy(m_Press,   "0");
    strcpy(m_AirIndex, "OK");
    strcpy(m_VoltageAC, "0");
    strcpy(m_VoltageDC,  "0");
    
    m_screenSensor = 0;
    m_curBrightness = 100;
    m_IsSleep = 0;
  
    m_TempSensor = 0;

	m_currCMD	 = 0;
	m_currInvCMD = 0;
}


/*

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}*/
void CMainAppl::ResetNextion()
{
	nexSerial.print("rest");
	nexSerial.write(0xFF);
	nexSerial.write(0xFF);
	nexSerial.write(0xFF);
	nexSerial.end();
	delay(1000);
}

void CMainAppl::InitNextionSerialSpeed()
{
	nexSerial.print("baud=57600");
	nexSerial.write(0xFF);
	nexSerial.write(0xFF);
	nexSerial.write(0xFF);
	nexSerial.end();
	delay(100);

	nexSerial.begin(57600);
	nexSerial.print("baud=57600");
	nexSerial.write(0xFF);
	nexSerial.write(0xFF);
	nexSerial.write(0xFF);
}



void CMainAppl::Init()
{
  SMESN("INIT MAIN PAGE");
  nexInit();
  
  //todo check
  ResetNextion();
  InitNextionSerialSpeed();
  
  cmdArduino.Init();
  delay(50);
  cmdArduino2.Init();

  m_Constructor.SetParent(this);

  m_currListenItem = 0;
  for (int i = 0 ; i < contMainItems; i++)
    m_nex_listen_list[i] = 0;

  btnPressAir = 0;
  btnKlimate = 0;
  btnDoor = 0;
  btnSettings = 0;

  //create btn Light
  btnLight = new NexButton(m_pageID, 1, "bLight");
  btnLight->attachPop(CMainAppl::ShowLight, this);
  m_nex_listen_list[m_currListenItem++] = btnLight;

  //create pressair
  btnPressAir = new NexButton(m_pageID, 2, "bPressAir");
  btnPressAir->attachPop(CMainAppl::ShowPressAir, this);
  m_nex_listen_list[m_currListenItem++] = btnPressAir;

  //create klimate
  btnKlimate = new NexButton(m_pageID, 3, "bKlimate");
  btnKlimate->attachPop(CMainAppl::ShowKlimate, this);
  m_nex_listen_list[m_currListenItem++] = btnKlimate;

  //create btn electric
  btnElectronikNetwork = new NexButton(m_pageID, 4, "bENetwork");
  btnElectronikNetwork->attachPop(CMainAppl::ShowElectric, this);
  m_nex_listen_list[m_currListenItem++] = btnElectronikNetwork;

  //create btn door
  btnDoor = new NexButton(m_pageID, 5, "bDoor");
  btnDoor->attachPop(CMainAppl::ShowDoor, this);
  m_nex_listen_list[m_currListenItem++] = btnDoor;

  //create btn settings
  btnSettings= new NexButton(m_pageID, 6, "bSettings");
  btnSettings->attachPop(CMainAppl::ShowSettings, this);
  m_nex_listen_list[m_currListenItem++] = btnSettings;


  m_voltSensor.voltage(SENSOR_VOLT_AC, 237, 1.7);
  m_voltSensor.calcVI(20, 185);
  
  //create sensor page
  m_pDate     = new NexText(m_pageID, 7,  "tDate");
  m_pTime     = new NexText(m_pageID, 8,  "tTime");
  m_pTempIn   = new NexText(m_pageID, 9,  "tTpO");
  m_pTempOut  = new NexText(m_pageID, 10, "tTpI");
  m_pPress    = new NexText(m_pageID, 11, "tPress");
  m_pPower    = new NexText(m_pageID, 12, "tPower");
  m_pVoltage  = new NexText(m_pageID, 13, "tVolt");
  m_pAmpers   = new NexText(m_pageID, 14, "tAmper");

  InvalidateAllSensors(m_pDate, m_pTime, m_pTempIn, m_pTempOut, m_pPress, m_pPower, m_pVoltage,m_pAmpers);

  //real time clock
  m_DateTime.begin();


  //setup screen sensor and timer
  m_screenSensor = new CDigitalSensor(2); // screen brightness sensor
  unsigned long tmSensScreen = 10;
  
  //set temperature IN/OUT sensor 
  m_TempSensor = new CTempSensor_DS18B20(SENSOR_TEMP_IN_OUT);
  m_TempSensor->getAddress(ThermometerIN, 0);
  m_TempSensor->getAddress(ThermometerOUT, 1);
  
  m_TempSensor->SetResolution(ThermometerIN);
  m_TempSensor->SetResolution(ThermometerOUT);

  //wake up display, if brightness < 100%
  sCmdWord w(CMD_SLEEP);
  w.SetAction(0);
  ProcessCMD(w);

  //say to all arduino about reinit 
  sCmdWord w1(SHOW_MAIN_MENU);
  ProcessCMD(w1);

  //invalidate zone 1
  ReadData(true);
}


void CMainAppl::Show()
{
   cmdArduino.SendCommand(SHOW_MAIN_MENU);
   cmdArduino2.SendCommand(SHOW_MAIN_MENU);
   CMainAppl::ActivePage = eMAIN;
   show();

   InvalidateAllSensors(m_pDate, m_pTime,  m_pTempIn,  m_pTempOut, 
                       m_pPress, m_pPower, m_pVoltage, m_pAmpers );
}

void CMainAppl::ShowByButton()
{
    CNexPageEx* ap = m_Constructor.GetObject(ActivePage);
    SMES("Show Screen #"); SMESN(ActivePage);
    ap->Show(); // show current screen
    
    InvalidateAllSensors(ap->m_pDate,  ap->m_pTime,  ap->m_pTempIn,  ap->m_pTempOut, 
                         ap->m_pPress, ap->m_pPower, ap->m_pVoltage, ap->m_pAmpers );
}

void CMainAppl::ShowLight(void* ptr)
{
  CMainAppl* ap = (CMainAppl*)ptr;
  ap->ActivePage = eLight;
  ap->ShowByButton();
  
  //send ask for invalidate
  ap->cmdArduino.SendCommand(SHOW_LIGHT_PAGE);
  ap->cmdArduino2.SendCommand(SHOW_LIGHT_PAGE);
}

void CMainAppl::ShowPressAir(void* ptr)
{
    CMainAppl* ap = (CMainAppl*)ptr;
    ap->ActivePage = ePressair;
    ap->ShowByButton();

    //send ask for invalidate
    ap->cmdArduino.SendCommand(SHOW_PRESSAIR_PAGE);
    ap->cmdArduino2.SendCommand(SHOW_PRESSAIR_PAGE);
}

void CMainAppl::ShowKlimate(void* ptr)
{
    CMainAppl* ap = (CMainAppl*)ptr;
    ap->ActivePage = eKlimate;
    ap->ShowByButton();

    //send ask for invalidate
    ap->cmdArduino.SendCommand(SHOW_KLIMATE_PAGE);
    ap->cmdArduino2.SendCommand(SHOW_KLIMATE_PAGE);
}

void CMainAppl::ShowElectric(void* ptr)
{
  CMainAppl* ap = (CMainAppl*)ptr;
  ap->ActivePage = eElectNetWork;
  ap->ShowByButton();
  
  //send ask for invalidate
  ap->cmdArduino.SendCommand(SHOW_ELECT_NET_PAGE);
  ap->cmdArduino2.SendCommand(SHOW_ELECT_NET_PAGE);
}

void CMainAppl::ShowDoor(void* ptr)
{
    CMainAppl* ap = (CMainAppl*)ptr;
    ap->ActivePage = eDoor;
    ap->ShowByButton();

    //send ask for invalidate
    ap->cmdArduino.SendCommand(SHOW_DOOR_PAGE);
    ap->cmdArduino2.SendCommand(SHOW_DOOR_PAGE);
}

void CMainAppl::ShowSettings(void* ptr)
{
    CMainAppl* ap = (CMainAppl*)ptr;
    ap->ActivePage = eSettings;
    ap->ShowByButton();

    //send ask for invalidate
    ap->cmdArduino.SendCommand(SHOW_SETTINGS_PAGE);
    ap->cmdArduino2.SendCommand(SHOW_SETTINGS_PAGE);
}

void CMainAppl::Process()
{ 
    m_screenSensor->ReadData();

    if(m_screenSensor->IsDetectedMotion()) //need inverse singnal
    {
        if(m_IsSleep)
        {
            sCmdWord w(CMD_SLEEP);
            w.SetAction(0);
            ProcessCMD(w);
            m_IsSleep = false;
        }        
    }
    else  
    {      
        if(!m_IsSleep)
        {
            sCmdWord w(CMD_SLEEP);
            ProcessCMD(w);
            m_IsSleep = true;
        }
    }

    ReadData();
    m_Constructor.GetObject(ActivePage)->ProcessInterface();

    ProcessSerial(cmdArduino);
    ProcessSerial(cmdArduino2);
}

void CMainAppl::ProcessSerial(CSendCmd &cmdArduino)
{
    uint8_t c = 0;

    while (cmdArduino.available() > 0)
    {   
        delay(10);
        c = cmdArduino.Read();

        if (c == 'L' || c == 'P' || c == 'K' || c == 'E' || c == 'D' || c == 'S' || c == 'M')
        {
            delay(10);
            if (cmdArduino.available() >= CMD_LEN - 1)
            {
                sCmdWord cmd;

                cmd.buff[0] = c;
                cmd.buff[1] = cmdArduino.Read();

                bool IsLongCmd = false;
                if(cmd.buff[1] == CMD_NUM || cmd.buff[1] == CMD_FLOAT)
                    IsLongCmd = true;

				if(cmd.buff[1] == CMD_INVALIDATE)
				{
					sCmdInvalidate *cmdinv = &m_incomingInvCmd[m_currInvCMD];
					*cmdinv = sCmdInvalidate(c);
					cmdinv->AddNextSymb(cmdArduino.Read());

					int symbCount = 0;
					if(cmdinv->GetReciver() == 'L')
					{
						if(cmdinv->GetMode() == 'A')
							symbCount = 16;
						else
						if(cmdinv->GetMode() == 'B')
							symbCount = 8;
					}
					else
					if(cmdinv->GetReciver() == 'E')
					{
						if(cmdinv->GetMode() == 'A')
							symbCount = 11;
					}
					
					while(symbCount > cmdArduino.available() )
						delay(2);

					for (int i = 0; i< symbCount; i++)
						cmdinv->AddNextSymb(cmdArduino.Read()); 
										
					m_currInvCMD++;
					
					if(m_currInvCMD >= 15)
					{
						ExecInvCMD();
						ExecCMD();
					}
				}
				else
				{
					if(IsLongCmd)
					{
						for (int i = 2; i< CMD_WORD_LEN; i++)
							cmd.buff[i] = cmdArduino.Read(); 

						cmd.buff[CMD_WORD_LEN - 1] = 0x0;
					}
					else
					{
						for (int i = 2; i < CMD_LEN; i++)
							cmd.buff[i] = cmdArduino.Read(); 
						cmd.buff[CMD_LEN] = 0x0;
					}
					cmd.EjectType();

					memmove(m_incomingCmd[m_currCMD].buff, cmd.buff, sizeof(cmd.buff));
					m_currCMD++;
					
					if(m_currCMD >= 49)
					{
						ExecInvCMD();
						ExecCMD();
					}
				}
            }
        }
    } 
	ExecInvCMD();
	ExecCMD();
}

void CMainAppl::ExecCMD()
{
	for(int i = 0; i < m_currCMD; i++)
	{
		ProcessCMD(m_incomingCmd[i]);
		m_incomingCmd[i].clear();
	}
	m_currCMD = 0;
}

void CMainAppl::ExecInvCMD()
{
	for(int i = 0; i < m_currInvCMD; i++)
	{
		ProcessInvalidateCMD(m_incomingInvCmd[i]);
		m_incomingInvCmd[i].clear();
	}
	m_currInvCMD = 0;
}

void CMainAppl::CMDExtFilter(sCmdWord &cmd)
{
    if(cmd.IsCommand(PA_TEXT_PRESS_CUR))
    {
        float val = 0.0;
        cmd.GetFloatNumber(val, m_Press);
    }
}

void CMainAppl::ProcessInvalidateCMD(sCmdInvalidate& cmd)
{
	SMES("Input cmd (p");
	SMES((int)ActivePage);
	SMES("): ");
	SMESN(cmd.buff);

	switch(ActivePage)
	{
		case eLight:
		{
			if(cmd.GetReciver() == 'L')
			{
				m_Constructor.GetObject(eLight)->ProcessInvalidateCMD(cmd);
				return;
			}
			break;
		}
		case eElectNetWork:
		{
			if(cmd.GetReciver() == 'E')
			{
				m_Constructor.GetObject(eElectNetWork)->ProcessInvalidateCMD(cmd);
				return;
			}
			break;
		}
	}
}

void CMainAppl::ProcessCMD(sCmdWord & cmd)
{
    SMES("Input cmd (p");
    SMES((int)ActivePage);
    SMES("): ");
    SMESN(cmd.buff);

    CMDExtFilter(cmd);

    switch(ActivePage)
    {
        case eLight:
        {
            if(cmd.GetReciver() == 'L')
            {
                m_Constructor.GetObject(eLight)->ProcessCMD(cmd);
                return;
            }
            break;
        }
        case ePressair:
        {
            if(cmd.GetReciver() == 'P')
            {
                m_Constructor.GetObject(ePressair)->ProcessCMD(cmd);
                return;
            }
            break;
        }
        case eKlimate:
        {
            if(cmd.GetReciver() == 'K')
            {
                m_Constructor.GetObject(eKlimate)->ProcessCMD(cmd);
                return;
            }
            break;
        }
        case eElectNetWork:
        {
            if(cmd.GetReciver() == 'E')
            {
                m_Constructor.GetObject(eElectNetWork)->ProcessCMD(cmd);
                return;
            }
            break;
        }
        case eDoor:
        {
            if(cmd.GetReciver() == 'D')
            {
                m_Constructor.GetObject(eDoor)->ProcessCMD(cmd);
                return;
            }
            break;
        }
        case eSettings:
        {
            if(cmd.GetReciver() == 'S')
            {
                m_Constructor.GetObject(eSettings)->ProcessCMD(cmd);
                return;
            }
            break;
        }
    }

    if(cmd.IsCommand(SHOW_MAIN_MENU))
      Show();
    else
    if(cmd.IsCommand(SHOW_LIGHT_PAGE))
      ShowLight(this);
    else
    if(cmd.IsCommand(SHOW_PRESSAIR_PAGE))
      ShowPressAir(this);
    else
    if(cmd.IsCommand(SHOW_KLIMATE_PAGE))
      ShowKlimate(this);
    else
    if(cmd.IsCommand(SHOW_ELECT_NET_PAGE))
      ShowElectric(this);
    else
    if(cmd.IsCommand(SHOW_DOOR_PAGE))
      ShowDoor(this);
    else
    if(cmd.IsCommand(SHOW_SETTINGS_PAGE))
      ShowSettings(this);
    if(cmd.IsCommand(CMD_SETBRT))
    {
        SMESN("SetBri");

		char szBuff[7] = {0};
        setBrightness(cmd.GetNumber(szBuff));
    }
    else
    if(cmd.IsCommand(CMD_SLEEP))
     {
        short act = cmd.GetAction();
        if(act == 1)
        {
            SMESN("Go to sleep..");
            m_curBrightness = getBrightness();
            setBrightness(0);
        }
        else
        {
                SMESN("Wake up..");
                if(m_curBrightness <= 0)
                m_curBrightness = 10;
                setBrightness(m_curBrightness);
        }
    }
}

uint32_t CMainAppl::getBrightness()
{
  sendCommand("get dim");
  uint32_t val = 100;
  if (recvRetNumber(&val))
  {
    SMES("Cur bri: ");
    SMESN(val);
    return val;
  }
  else
    return 0;
}

void CMainAppl::setBrightness(uint32_t val)
{
  if(val > 100 || val < 0)
    val = 100;

  char buff[20]={0};
  sprintf(buff, "dim=%d", val);
  sendCommand(buff);
}



float CalcPress(int Value)
{
    float tmp = constrain(Value, 0, 1023);
    float t2 = (tmp/ 1024) * 23.0f; 

    if(t2 < 0) return 0;
    return t2;
}

float CalcPower(int Value)
{
    return Value * 1.0f;
}

float CalcVoltage(int Value)
{
  float tmp = constrain(Value, 0, 1023);
  float t2 = (tmp/ 1024) * 23.0f; 

  if(t2 < 0) return 0;
  return t2;
}

void CMainAppl::ReadAndSetDateAndTime()
{
  static uint8_t prevYYYY = 0;
  static uint8_t prevMN = 0;
  static uint8_t prevDD = 0;
  
  static uint8_t prevHH = 0;
  static uint8_t prevMM = 0;

  m_DateTime.gettime();
  SMESN("GetDate");
    
  if(prevHH != m_DateTime.Hours || m_DateTime.minutes != prevMM)
  {
      prevHH = m_DateTime.Hours;
      prevMM = m_DateTime.minutes;        
          sprintf(m_timer, "%.2d:%.2d", prevHH, prevMM);
      
      NexText *item = m_Constructor.GetObject(ActivePage)->m_pTime;
          if(item)
              item->setText(m_timer);
  }
  
  if(prevYYYY != m_DateTime.year || prevMN != m_DateTime.month || prevDD != m_DateTime.day)
  {
      prevYYYY = m_DateTime.year;
      prevMN   = m_DateTime.month;
      prevDD   = m_DateTime.day;
      
      sprintf(m_date, "%.2d/%.2d/%.2d", prevDD, prevMN, prevYYYY);
  
      NexText *item = m_Constructor.GetObject(ActivePage)->m_pDate;
      if(item)
          item->setText(m_date);
  }
}

bool CMainAppl::ReadTempIn() 
{ 
    static int prevvalue = 0;
    float value = m_TempSensor->GetSensorValueCelse(ThermometerIN);
    int nev =(int)(value * 10); 
    if( nev == prevvalue)
      return false;
    
    char buff[20]={0};
    dtostrf(value, 5,1, buff);
    tsLeft(buff, strlen(buff));
    
    sprintf(m_TempIn, "%s", buff);
    SMES("TempIN: ");
    SMESN(m_TempIn);
    prevvalue = nev;
    
    return true;
}

bool CMainAppl::ReadTempOut() 
{ 
    static int prevvalue = 0;
    float value = m_TempSensor->GetSensorValueCelse(ThermometerOUT);
    
    int nev =(int)(value * 10); 
    if( nev == prevvalue)
      return false;
    
    char buff[20]={0};
    dtostrf(value, 5,1, buff);
    tsLeft(buff, strlen(buff));
    
    sprintf(m_TempOut, "%s", buff);
    
    SMES("TempOut: ");
    SMESN(m_TempOut);
    prevvalue = nev;
    
    return true;
}


bool CMainAppl::ReadAQI(int value) 
{ 
    static int prevValue = 0;
    
    if(value == -1) return false;
    //value = CalcPower(analogRead(SENSOR_POWER));
    
    if(prevValue != value)
    {
        prevValue = value;
        
        switch(value)
        {
            case 1:
                sprintf(m_AirIndex, "OK", value);
                break;
            case 2:
                sprintf(m_AirIndex, "MID", value);
                break;
            case 3:
                sprintf(m_AirIndex, "HIGH", value);
                break;
        }
        return true;
    }
    
    return false;
}

bool CMainAppl::ReadVoltageAC() 
{
    static int prevValue = 0;
    m_voltSensor.calcVI(20, 185);

      static short k = 0;
      static float avg[10] = {0,0,0,0,0,0,0,0,0,0};
      if(k >= 9) k = 0;
      avg[++k] = m_voltSensor.Vrms ;

      float sum = 0.0f;
      for(int i = 0; i < 10; i++)
        sum += avg[i];
      sum /= sizeof(avg) / sizeof(float);
        
    int value = m_voltSensor.Vrms < 10.0f ? 0 : (int)m_voltSensor.Vrms;
    if(prevValue != value)
    {
        prevValue = value;
        sprintf(m_VoltageAC, "%d", value);
        return true;
    }
    return false;
}

// рассчёт делителя
// const float R1 = 30000; // 30 kOm
// const float R2 = 7500;  // 7.5 kOm
// коэффициент для расчёта напряжения на входе делителя
//const float dividerRatio = (R1+R2)/R2; 

bool CMainAppl::ReadVoltageDC() 
{ 
    static double prevValue = 0.0f;
    
      double Vcc = readVcc();
      double voltRatio = Vcc/1023 ;
  
      int VoltageSensorValue = (analogRead(SENSOR_VOLT_DC));
      double value = VoltageSensorValue * voltRatio * 5.0f; /*dividerRatio = 5*/
    if(prevValue != value)
    {
        memset(m_VoltageDC, 0, sizeof(m_VoltageDC));
        prevValue = value;      

            dtostrf(value, 4, 1, m_VoltageDC);
        return true;
    }
    return false;
}

void CMainAppl::ReadData(bool Force)
{
	
	if(!Force && millis()%10000 != 1 ) //перечитываем время каждые 10 сек, чтобы не грузить шину
        return;
	
    ReadAndSetDateAndTime();
        
    m_TempSensor->requestTemperatures();
    if(ReadTempIn())
    {
        NexText *item = m_Constructor.GetObject(ActivePage)->m_pTempIn;
        if(item)
            item->setText(m_TempIn);
    }

    if(ReadTempOut())
    {
        NexText *item = m_Constructor.GetObject(ActivePage)->m_pTempOut;
        if(item)
            item->setText(m_TempOut);
    }
    
      if(ReadAQI())
      {
          NexText *item = m_Constructor.GetObject(ActivePage)->m_pPower;
          if(item)
              item->setText(m_AirIndex);
      }

    NexText *item = m_Constructor.GetObject(ActivePage)->m_pPress;
    if(item)
       item->setText(m_Press);
    

    if(ReadVoltageAC())
    {
        NexText *item = m_Constructor.GetObject(ActivePage)->m_pVoltage;
        if(item)
            item->setText(m_VoltageAC);
    }

    if(ReadVoltageDC())
    {
        NexText *item = m_Constructor.GetObject(ActivePage)->m_pAmpers;
        if(item)
        {
          item->setText(m_VoltageDC);
        }
    }
}

void CMainAppl::InvalidateAllSensors(NexText* pDate, NexText* pTime, NexText* pTempIn, NexText* pTempOut, 
  NexText* pPress, NexText* pPower, NexText* pVoltage, NexText* pAmpers)
{
  SMESN("Invalidate All Zone 1 data");
  if(pDate)
    pDate->setText(m_date);

  if(pTime)
    pTime->setText(m_timer);

  if(pTempIn)
    pTempIn->setText(m_TempIn);

  if(pTempOut)
    pTempOut->setText(m_TempOut);

  if(pPress)
    pPress->setText(m_Press);

  if(pPower)
    pPower->setText(m_AirIndex);

  if(pVoltage)
    pVoltage->setText(m_VoltageAC);

  if(pAmpers)
    pAmpers->setText(m_VoltageDC);
}


CMainAppl::~CMainAppl(void)
{
  if (btnLight)
    delete btnLight;

  if(btnPressAir)
    delete btnPressAir;

  if(btnKlimate)
    delete btnKlimate;

  if(btnElectronikNetwork)
    delete btnElectronikNetwork;

  if(btnDoor)
    delete btnDoor;

  if(btnSettings)
    delete btnSettings;
    
  if(m_screenSensor)
    delete m_screenSensor;
  
  if(m_TempSensor)
     delete m_TempSensor;
}

// функция вычисления напряжения питания
float CMainAppl::readVcc() {
  byte i;
    float result = 0.0;
    float tmp = 0.0;

#ifndef WIN32
      ADCSRA |= (1<<ADEN);
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
      ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
      ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
      // works on an Arduino 168 or 328
      ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif

    delay(3); // Wait for Vref to settle
    for (i = 0; i < 5; i++) {
        ADCSRA |= _BV(ADSC); // Start conversion
        while (bit_is_set(ADCSRA,ADSC)); // measuring

        uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
        uint8_t high = ADCH; // unlocks both

        tmp = (high << 8) | low;
        tmp = (1.08 * 1023.0) / tmp;
        result = result + tmp;
        delay(5);
    }
    result = result / 5;
#endif

    return result;
}
