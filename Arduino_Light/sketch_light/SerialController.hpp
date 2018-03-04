#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "SerialCmd.h"
#include "PanelIncl.hpp"

extern CElecPanelController*    m_panelElect;
extern CLightPanelController*    m_panelLight;
extern CPressAirPanelController*    m_panelPressAir;

/*
   class for handle serial cmd
*/
class CSerialController
{
public:
    CSerialController() 
        : cmdArduino(CSendCmd::eSerial2, 
   #ifdef DEBUG_INFO
        true
   #else
        false
   #endif
        )
    {

    }

    static long ActivepaPage;
    
    void Init()
    {    
        cmdArduino.Init();
    }

    inline long ProcessSerial()
    {
#ifndef WIN32
		uint8_t c = 0; 
   
       while (cmdArduino.available() > 0)
       {   
         SMES("IN CMD[");
         SMES(CSerialController::ActivepaPage);
         SMES("]: ");
         
         delay(10);
         c = cmdArduino.Read();
        
         if (BEGIN_CMD == c)
         {
           delay(10);
           if (cmdArduino.available() >= CMD_WORD_LEN - 2)
           {
              int val = cmdArduino.available();
                     
              sCmdWord cmd;
              cmd.buff[0] = BEGIN_CMD;
              for (int i = 1; i < CMD_WORD_LEN - 1; i++)
                cmd.buff[i] = cmdArduino.Read(); 
              
              cmdArduino.flush();
              
              SMES(cmd.buff);
              SMES("[");
              SMES(cmd.GetReciver());
              SMESN("]");
              
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

#endif
      return 1;  
    }

    CSendCmd cmdArduino;
};


#endif 
