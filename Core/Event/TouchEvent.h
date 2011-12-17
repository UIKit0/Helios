//
//  TouchEvent.h
//  Helios
//
//  Created by James Hurley on 9/20/11.
//

#ifndef Helios_TouchEvent_h
#define Helios_TouchEvent_h
#include <Event/BaseEvent.h>


namespace helios
{
    struct Touch
    {
        uint64_t timestamp;
        uint16_t x, y;
        uint8_t flags;   // 0x10 represents 'hit'; 0x8->0x1 represents 'phase' bits (see UITouchPhase).
        uint8_t tapCount;
    };
    class TouchEvent : public BaseEvent<std::vector<Touch> >
    {        
    public:
        TouchEvent() : BaseEvent<std::vector<Touch> >()
        {};
        TouchEvent( std::string & name, std::vector<Touch>& touch ) : BaseEvent<std::vector<Touch> > ()
        {
            mName = name;
            mData = touch;
            mTarget = e::kEventTargetTouch;           
            
        };
        TouchEvent( std::string const & name, std::vector<Touch>& touch) : BaseEvent<std::vector<Touch> > ()
        {
            mName = name;
            mData = touch;
            mTarget = e::kEventTargetTouch;
        };
        ~TouchEvent() {}; 
        
    };
    
};

#endif
