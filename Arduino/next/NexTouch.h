#ifndef __NEXTOUCH_H__
#define __NEXTOUCH_H__


typedef void (*NexTouchEventCb)(void *ptr);

class NexTouch
{
public:
    NexTouch(){   m_push = 0; };
    void attachPush(NexTouchEventCb push, void *ptr = 0)
    {
        m_push = push;
        m_funcParam = ptr;
    }

    void attachPop(NexTouchEventCb push, void *ptr = 0)
    {
    }
    

    
    virtual ~NexTouch(){}

    NexTouchEventCb m_push;
    void* m_funcParam;
};

#endif /* #ifndef __NEXTOUCH_H__ */
