#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "SerialCmd.h"
#include "PanelIncl.hpp"

extern CDoorPanelController*    m_panelLight;

/*
   class for handle serial cmd
*/
class CSerialController
{
public:
    CSerialController() 
        : cmdArduino(CSendCmd::eSerial3, 
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
              
              if(cmd.GetReciver() == 'D')
              {
                  if(cmd.IsCommand(SHOW_DOOR_PAGE))
                     CSerialController::ActivepaPage = eDoor;
                  m_panelLight->ProcessSerialCMD(cmd);
              }
			        if(cmd.GetReciver() == 'K')
			        {
				         if(cmd.IsCommand(SHOW_KLIMATE_PAGE))
					          CSerialController::ActivepaPage = eKlimate;
				         m_panelLight->ProcessSerialCMD(cmd);
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
