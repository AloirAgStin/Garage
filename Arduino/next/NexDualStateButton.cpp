
#include "NexDualStateButton.h"


NexDSButton::NexDSButton(uint8_t pid, uint8_t cid, const char *name) 
    : NexTouch()
{
    strcpy(m_name, name);
    m_cid = cid;
    m_pid =  pid;
    m_val = 0;

}

bool NexDSButton::getValue(uint32_t *number)
{
    *number = m_val;
    return true;
}

bool NexDSButton::setValue(uint32_t number)
{
    m_val = number;
    return true;
}
