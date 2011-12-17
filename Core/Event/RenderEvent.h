//
//  RenderEvent.h
//  Helios
//
//  Created by James Hurley on 9/21/11.
//

#ifndef Helios_RenderEvent_h
#define Helios_RenderEvent_h
#include <Event/BaseEvent.h>
#include <Render/RenderCommand.h>

namespace helios
{
    class RenderEvent : public BaseEvent<RenderCommand>
    {
    public:
        RenderEvent(RenderCommand r) : BaseEvent<RenderCommand>()
        {
            mName = e::kEventRenderCommand;
            mTarget = e::kEventTargetRender;
            mData = r;
           
        };
    };
    
};


#endif
