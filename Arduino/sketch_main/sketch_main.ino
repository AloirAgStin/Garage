#include "arduino.h"
#include "Nextion.h"
#include "MainAppl.h"

CMainAppl *app = 0;
  
void setup()
{
  app = new CMainAppl("pageMain");  
  app->Init();
  
  SMES("Free memory:");
  SMESN(memoryFree());
  SMESN("------");
}
 
void loop() 
{
  app->Process();
}





