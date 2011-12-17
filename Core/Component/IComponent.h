//
//  IComponent.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_IComponent_h
#define Helios_IComponent_h

#include <string>
#include <vector>
#include <map>
#include <common.h>
#include <Event/HEvent.h>
#include <boost/property_tree/ptree.hpp>

namespace helios
{
    typedef unsigned char byte;
    
    struct elevation_t
    {
        struct {
            byte entity : 1;
            byte layer : 1;
            byte scene : 1;
        } push;
        struct
        {
            byte entity : 1;
            byte layer : 1;
            byte scene : 1;
        } pull;
    };
    
    
    class IEntity;
    class IEvent;
    class IComponent
    {
        
    public:
        virtual ~IComponent() {};
        
        // void Update(IEvent*)
        // Gives the layer an opportunity to push an update event, 
        // component can then query the owner entity for relevant events in its
        // event pool.
        virtual void Update(uint64_t)  = 0;
        
        // void Process()
        // Components process data received in Update phase.
        virtual void Process() = 0;
        
        // void DispatchEvents()
        // Components dispatch events to Entity event pools
        virtual void DispatchEvents() = 0;
        
        // IEvent& operator()
        // For polling (such as in the case of the renderer taking data from
        // renderables.
        virtual IEvent_ptr operator()() = 0;
        
        //virtual void ComponentRegistered(IComponent* component) = 0;
        //virtual void ThisComponentRegistered(std::map<std::string, IComponent*> components) = 0;

        virtual IEntity& Owner() = 0;
        virtual std::string Name() = 0;
        
        // Take a property tree to generate components from XML or JSON.
        virtual void Setup(boost::property_tree::ptree const & pt) = 0;
        
        virtual void SetEventElevation(elevation_t elevation) = 0;
        
        virtual bool IsActive() = 0;
        virtual void SetActive(bool) = 0;
    };    
};

#endif
