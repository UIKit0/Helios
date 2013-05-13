//
//  ILayer.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_ILayer_h
#define Helios_ILayer_h

#include <Memory/Pool.h>
#include <Event/Events.h>
#include <map>
#include <vector>
#include <string>
#include <common.h>
#include <Scene/IScene.h>

namespace helios
{
    class IComponent;
    class IRender;
    class ILayer
    {
      
    public:
        virtual ~ILayer() {} ;
        
        virtual Pool& GetPool() = 0;
        virtual ILayer* OwnsComponent(IComponent* component, std::string name) = 0;
        
        virtual void SetViewport ( int x, int y, int w, int h, int dpi = 72 ) = 0 ;
        virtual void GetViewport ( int& x, int& y, int &w, int &h) = 0;
        
        virtual void onActive(uint64_t time) = 0;
        virtual void onDeactive() = 0;
        
        virtual void UpdateLayer() = 0;           
        virtual void Update(uint64_t time) = 0;
        virtual void Process() = 0;
        virtual void DispatchEvents() = 0;
        virtual void Render(uint64_t time) = 0;
        
        virtual void SetRenderer(IRender* renderer) = 0;
        virtual std::shared_ptr<IRender> GetRenderer() = 0;
        
        virtual void RegisterComponent(IComponent* component) = 0;
        virtual void RegisterComponents(std::map<std::string, std::vector<IComponent*> >) = 0;
        virtual void UnRegisterComponent(IComponent* component) = 0;
        virtual void UnRegisterComponent(IComponent* component, std::string mask) = 0;
        virtual void UnRegisterComponents(std::map<std::string, std::vector<IComponent*> >) = 0;
        
        virtual unsigned GetShader(std::string) = 0;
        virtual unsigned GetCurrentShader() = 0;
        
        virtual unsigned GetVAO(std::string) = 0;
        virtual unsigned GetCurrentVAO() = 0;
        
        virtual std::map<std::string, int>& Uniforms() = 0;
        
        virtual void PushEvent(IEvent_ptr e) = 0;
        virtual std::vector<IEvent_ptr> & GetEvents( std::string const & target) = 0;
        virtual IScene& GetOwner() = 0;
        
        virtual bool IsActive() = 0;
    };
    
};

#endif
