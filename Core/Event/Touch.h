//
//  Touch.h
//  Helios
//
//  Created by James Hurley on 10/12/11.
//

#ifndef Helios_Touch_h
#define Helios_Touch_h
#include <vector>
#include <stdint.h>

namespace helios
{
    struct Touch
    {
        uint64_t timestamp;
        uint16_t x, y;
        uint8_t flags;   // 0x10 represents 'hit'; 0x8->0x1 represents 'phase' bits (see UITouchPhase).
        uint8_t tapCount;
        Touch() {};
        Touch(Touch const & rhs)
        {
            timestamp = rhs.timestamp;
            x = rhs.x;
            y = rhs.y;
            flags = rhs.flags;
            tapCount = rhs.tapCount;
            
        };
        
        Touch& operator=(Touch const & rhs)
        {
            timestamp = rhs.timestamp;
            x = rhs.x;
            y = rhs.y;
            flags = rhs.flags;
            tapCount = rhs.tapCount;
            return *this;
        };
    };
    
    struct Touches
    {
        std::vector<Touch> touches;
        Touches(Touches const & rhs)
        {
            touches = rhs.touches;
        };
        Touches() {};
        Touches& operator= ( Touches const & rhs)
        {
            touches.reserve(rhs.touches.size());
            touches = rhs.touches;
            return *this;
        };
        Touch & operator[] ( size_t n )
        {
            return touches[n];
        };
    };
    
    typedef struct
    {
        uint16_t lshift_down: 1;
        uint16_t rshift_down: 1;
        uint16_t lctrl_down:  1;
        uint16_t rctrl_down:  1;
        uint16_t lalt_down:   1;
        uint16_t ralt_down:   1;
        uint16_t lcmd_down:   1;
        uint16_t rcmd_down:   1;
    } modifier_flags_t;
    struct KeyEvent
    {
        union {
            struct {
                modifier_flags_t flags;
                uint16_t keyCode;   
            };
            uint32_t mask;
        };
    };

};
#endif
