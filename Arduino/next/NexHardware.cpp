#include "NexHardware.h"


bool nexInit(void)
{
   return true;
}

void nexLoop(NexTouch *nex_listen_list[])
{ 
    int i = 0;
    while(nex_listen_list[i]!= 0)
    {
        /*
        NexDSButton *btn = (NexDSButton*)nex_listen_list[i];
        void *ptr = btn->m_funcParam;
        btn->m_push(ptr);
        
        */
        i++;
    }
}

