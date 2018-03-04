#ifndef __NEXBUTTON_H__
#define __NEXBUTTON_H__

#include "NexTouch.h"
#include "NexHardware.h"


typedef int uint32_t;
typedef int uint8_t;
typedef int uint16_t;


class NexButton: public NexTouch
{
public: /* methods */

    NexButton(uint8_t pid, uint8_t cid, const char *name);

    bool Set_background_color_bco(uint32_t number) { return true; }
};


class NexText: public NexTouch
{
public: /* methods */

    NexText(uint8_t pid, uint8_t cid, const char *name);
    
    //uint16_t getText(char *buffer, uint16_t len);
    bool setText(const char *buffer){ return true;}
    
};

#endif /* #ifndef __NEXBUTTON_H__ */
