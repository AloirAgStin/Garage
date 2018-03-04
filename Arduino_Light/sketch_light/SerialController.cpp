#include "SerialController.h"

extern CElecPanelController*    m_panelElect;
extern CLightPanelController*    m_panelLight;
extern CPressAirPanelController*    m_panelPressAir;


long CSerialController::ActivepaPage = eMAIN;

CSerialController::CSerialController() 
        : cmdArduino(CSendCmd::eSerial2, 
   #ifdef DEBUG_INFO
        true
   #else
        false
   #endif
        )
    {

    }

    
void CSerialController::Init()
{    
    cmdArduino.Init();
}

long CSerialController::ProcessSerial()
{
    uint8_t c = 0; 
   
    while (cmdArduino.available() > 0)
    {   
        delay(10);
        c = cmdArduino.Read();
        
        if (c == 'L' || c == 'P' || c == 'K' || c == 'E' || c == 'D' || c == 'S' || c == 'M')
        {
        delay(10);
        if (cmdArduino.available() >= CMD_LEN -1)
        {
            sCmdWord cmd;
                    
            cmd.buff[0] = c;
            cmd.buff[1] = cmdArduino.Read();
            
            bool IsLongCmd = false;
            if(cmd.buff[1] == CMD_NUM || cmd.buff[1] == CMD_FLOAT)
                IsLongCmd = true;

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
            
            SMES("IN CMD [");
            SMES(CSerialController::ActivepaPage);
            SMES("]->");
            SMES("[");
            SMES(cmd.GetReciver());
            SMES("]: ");
            SMES(cmd.buff);
            SMESN("");

            if(cmd.IsCommand(SHOW_MAIN_MENU))
                CSerialController::ActivepaPage = eMAIN; 
              
            if(cmd.GetReciver() == 'L')
            {
                if(cmd.IsCommand(SHOW_LIGHT_PAGE))
                    CSerialController::ActivepaPage = eLight;
                m_panelLight->ProcessSerialCMD(cmd);
            }
            else
            if(cmd.GetReciver() == 'E')
            {     
                if(cmd.IsCommand(SHOW_ELECT_NET_PAGE))
                    CSerialController::ActivepaPage = eElectNetWork;
                m_panelElect->ProcessSerialCMD(cmd);
            }  
            else
            if(cmd.GetReciver() == 'P')
            {     
                if(cmd.IsCommand(SHOW_PRESSAIR_PAGE))
                    CSerialController::ActivepaPage = ePressair;
                m_panelPressAir->ProcessSerialCMD(cmd);
            }
        }
    }
    } 

#ifndef WIN32
    #endif
    return 1;  
}
