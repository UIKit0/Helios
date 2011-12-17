//
//  BaseComponent.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_BaseComponent_h
#define Helios_BaseComponent_h

#include <Event/Events.h>
#include <Memory/Pool.h>
#include <Layer/ILayer.h>
#include <common.h>
#include <Entity/IEntity.h>
#include <Component/IComponent.h>
#include <World/SceneManager.h>
namespace helios
{

    class BaseComponent : public IComponent
    {
        
    protected:
        
        IEntity& mOwner;
        
        std::string  mName;
    
        elevation_t mEventElevation = { { 1, 0, 0 }, { 1, 0, 0 } };
        bool mIsActive = true;
        
    public:
    
        BaseComponent(IEntity& owner) : mOwner(owner)
        { 
            //mCurrentEvent = 0L;
            mName = e::kComponentBase;
        };
        ~BaseComponent()
        { D_PRINT("Destruct"); };
        
        virtual void Process()
        { 
           
        };
        virtual void Update(uint64_t) 
        {
            
        };
        virtual void DispatchEvents()
        { 
            // mOwner.PushEvent(mCurrentEvent);
        };
        std::string Name() 
        { 
            return mName ; 
        } ;
        virtual IEvent_ptr operator() () = 0;
        
        IEntity& Owner() { return mOwner; };
        void * operator new ( size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentBase, size);
        };
        void operator delete(void* ptr)
        {
            // Since we can't really pass much information to the delete
            // operator, we will attempt to discover the owner of the component
            // and release it from the appropriate pool.
            
            ILayer* owner;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentBase);
                
            }
            
           
            owner->GetPool().release(ptr, e::kComponentBase);
            
        };
        virtual void Setup(boost::property_tree::ptree const & pt)
        {
            
        };
        virtual void SetEventElevation(elevation_t elevation)
        {
            mEventElevation = elevation; // Always elevate to entity at least.            
        };        
        virtual bool IsActive() 
        {
            return mIsActive;
        };
        virtual void SetActive(bool isActive)
        {
            mIsActive = isActive;
        };
    };
    
};

#endif
