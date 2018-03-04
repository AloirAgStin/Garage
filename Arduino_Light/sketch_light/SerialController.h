#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "SerialCmd.h"
#include "PanelIncl.hpp"

/*
   class for handle serial cmd
*/
class CSerialController
{
public:
    CSerialController();
    static long ActivepaPage;
    
    void Init();
    long ProcessSerial();

    CSendCmd cmdArduino;
};


#endif 
