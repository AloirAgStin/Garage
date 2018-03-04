
#ifndef __NEXDSBUTTON_H__
#define __NEXDSBUTTON_H__

#include "NexHardware.h"
#include "../sketch_main/SerialCmd.h"

typedef int uint32_t;
typedef int uint8_t;
typedef int uint16_t;

class NexDSButton : public NexTouch
{
public: /* methods */
    NexDSButton(uint8_t pid, uint8_t cid, const char *name);
    
    bool getValue(uint32_t *number);
    bool setValue(uint32_t number);
    
    bool setText(const char *buffer){ return true;}
    
    
    uint8_t m_pid;
    uint8_t m_cid;
    char m_name[250];

    uint32_t m_val;
};


#endif /* #ifndef __NEXDSBUTTON_H__ */
