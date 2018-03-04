#include "PressAirPanelController.h"
#include "SerialController.h"
#include "GlobalDef.h"

#define PRESS_MAX_MAX 8.0f
#define PRESS_OUT_MAX 5.0f
#define BUTTON_STEP 0.5

short CPressAirPanelController::SetupObjects()
{
    m_currPress = 0.0;
    m_maxPress  = 7.0; //max press 8
    
    short nextSlot = 0; //NOT MODIFY
    
    short PIN_PRESSAIR = 50; //high signal
    m_compressor = CCompressor(PIN_PRESSAIR, "prs", PA_BTN_COMPRESSOR);
    
    short PIN_SINK = 12;
    m_sink = CSinkBtn(PIN_SINK, "kds", PA_BTN_CONDAUTO);
    
    
    //pressure sensor
    m_pressSens = CAnalogReader(A0);
    
    return 1;
}

void CPressAirPanelController::SendCMD(sCmdWord* cmd)
{
    //if active page not 4, return
    if(CSerialController::ActivepaPage != ePressair)
    {
        if(cmd->IsCommand(PA_TEXT_PRESS_CUR))
            cmdArduino->SendCommand(cmd);
        return;
    }

    cmdArduino->SendCommand(cmd);
}

void CPressAirPanelController::ProcessSerialCMD(sCmdWord &cmd)
{
    bool ret = false;
    if(cmd.IsCommand(SHOW_PRESSAIR_PAGE))
        InvalidateScreen();
    else
    if(cmd.IsName(m_compressor.m_Name))
    {
        int action = cmd.GetAction();
        if(action == 0)
            m_compressor.SetActive(false);
        else
            m_compressor.SetActive(true);
        SendCMD(m_compressor.GetCmdForInvalidateBtn());
    }
    else
    if(cmd.IsCommand(PA_BTN_SINK)) // press sink
        m_sink.Execute();
    else
    if(cmd.IsName("kda")) // sink auto
    {    
        if(cmd.GetAction() == 1)
            ret = m_sink.SetAuto(false);
        else
            ret = m_sink.SetAuto(true);
        if(ret)
        {
            SendCMD(m_sink.GetCmdForInv());
        }
    }
    else
    if(strncmp(cmd.buff + 1, "mp", 2) == 0) //
    {
        if(cmd.buff[3] == 'a')
            ret = setPress(1, BUTTON_STEP);
        else
        if(cmd.buff[3] == 'm')
            ret = setPress(1, -BUTTON_STEP);
        if(ret)
        {
            sCmdWord cmd2(PA_TEXT_PRESS_MAX, m_maxPress, 1);
            SendCMD(&cmd2);
        }
    }
}

bool CPressAirPanelController::setPress(short num, float value)
{
    bool ret = false;

    float newVal = 0.0;

	if(m_maxPress + value > PRESS_MAX_MAX)
        newVal = PRESS_MAX_MAX;
    else
    if(m_maxPress + value < 1.0f)
        newVal = 1.0f;
    else
        newVal = m_maxPress + value;

        ret = !IsDoubleEqual(newVal, m_maxPress);
        m_maxPress = newVal;

    return ret;
}

bool CPressAirPanelController::readPressAir(short num, float& value, bool _force)
{
	if(!_force)
	{
		static unsigned long lastdraw = 0;
		if(millis() - lastdraw < 5000)
			return false;
	}

    float tempPress = 0.0;   
	const int countCalc = 10;
    for (int i = 0; i < countCalc; i++) 
    {
        float val = m_pressSens.ReadValue();
        if(val < 98)
            val = 98;    
        tempPress += ( val - 98)/ 66.125; // <<----- pressair formula
		delay(30);
    }

    //new value
    tempPress = tempPress / (float)countCalc;
	if(IsDoubleEqual(tempPress, value))
		return false;

    value = tempPress;
    return true;
}

int CPressAirPanelController::Process()
{   
    if(readPressAir(1, m_currPress))
    {
		//sCmdWord cmd(PA_TEXT_PRESS_CUR, m_currPress, 1);
        //SendCMD(&cmd);
        //delay(50);
    }

    m_compressor.Process(m_currPress, m_maxPress);
    m_sink.Process();

    return 1;
}


void CPressAirPanelController::Show()
{
    InvalidateScreen();
}

void CPressAirPanelController::InvalidateScreen()
{
    SMESN("Inv PRESSAIR");
    int inDelay = 100;

    SendCMD(m_compressor.GetCmdForInvalidateBtn());
    delay(inDelay);

    {
        readPressAir(1, m_currPress, true);
        sCmdWord cmd(PA_TEXT_PRESS_CUR, m_currPress, 1);
        SendCMD(&cmd);
        delay(inDelay);
    }

    {
        sCmdWord cmd(PA_TEXT_PRESS_MAX, m_maxPress, 1);
        SendCMD(&cmd);
        delay(inDelay);
    }
}

CPressAirPanelController::CPressAirPanelController(CSendCmd *port) 
    : m_pressSens(A0)
{
    cmdArduino      = port;
}

short CPressAirPanelController::Init()
{
    m_currPress = 0.0;
    m_maxPress  = 0.0;

    short ret = 0;
    ret = SetupObjects();

    if(ret != 1) return ret;    


    return 1;
}

CPressAirPanelController::~CPressAirPanelController(void)
{    
}
