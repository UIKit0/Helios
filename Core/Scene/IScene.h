//
//  IScene.h
//  Helios
//
//  Created by James Hurley on 9/22/11.
//

#ifndef Helios_IScene_h
#define Helios_IScene_h

#include <map>
#include <string>
#include <Event/Events.h>
#include <common.h>

namespace helios
{
    class ILayer;
    class IRender;
    
    class IScene
    {
    public:
        virtual ~IScene() {} ;
        
        virtual size_t RegisterLayer(std::string,ILayer*) = 0;
        virtual size_t UnRegisterLayer(std::string,ILayer*) = 0;
        virtual std::map<std::string,ILayer*> & GetLayers() = 0;
        
        virtual void onScreenTouchBegan(HEvent<Touches>&) = 0;
        virtual void onScreenTouchMoved(HEvent<Touches>&) = 0;
        virtual void onScreenTouchEnded(HEvent<Touches>&) = 0;
        virtual void onScreenTouchCanceled() = 0;
        
        virtual void onActive(uint64_t) = 0;
        virtual void onDeactive() = 0;
        
        virtual void UpdateScene() = 0;
        virtual void UpdateLayer() = 0;         // If new objects need to be created, they go here.
        virtual void DispatchEvents() = 0;      // Dispatch processing from previous frame.
        virtual void Update(uint64_t time) = 0; // This is where components get updated data required for processing
        virtual void Process() = 0;             // Process data.
        virtual void Render(uint64_t time) = 0; // Render.
        
        virtual void SetRenderer(IRender* renderer) = 0;
        virtual void SetViewport(short x, short y, short w, short h, int dpi) = 0;
        
        virtual void NetworkRecv(IEvent_ptr e) = 0;
        virtual void NetworkSend(IEvent_ptr e) = 0;
        virtual void onKeyDown(helios::HEvent<helios::KeyEvent>& k) = 0;
        virtual void onKeyUp(helios::HEvent<helios::KeyEvent>& k) = 0;
        virtual void onKeyModifierChanged(helios::HEvent<helios::KeyEvent>& k) = 0;
        
        virtual void PushEvent(IEvent_ptr e) = 0;
        virtual std::vector<IEvent_ptr> & GetEvents( std::string const & target) = 0;
    };
};


#endif
