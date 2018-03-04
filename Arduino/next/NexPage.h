#ifndef __NEXPAGE_H__
#define __NEXPAGE_H__

#include "NexTouch.h"
#include "NexHardware.h"
/**
 * @addtogroup Component 
 * @{ 
 */

/**
 * A special component , which can contain other components such as NexButton, 
 * NexText and NexWaveform, etc. 
 */
class NexPage: public NexTouch
{
public: /* methods */
    NexPage(uint8_t pid, uint8_t cid, const char *name);
    
    bool show(void);
};
/**
 * @}
 */

#endif /* #ifndef __NEXPAGE_H__ */
