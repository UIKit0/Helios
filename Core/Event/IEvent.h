//
//  IEvent.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_IEvent_h
#define Helios_IEvent_h
#include <stdint.h>
#include <string>
#include <iostream>
#include <common.h>

namespace helios
{

    class IEvent
    {
    public:
        virtual ~IEvent() {} ;
        //virtual void * Sender() = 0;
        //virtual void SetSender(void*) = 0;
        virtual void SetTarget(std::string target) = 0;
        virtual void SetName( std::string name ) = 0;
        virtual std::string & GetName() = 0;
        virtual std::string & GetTarget() = 0;
        virtual std::string & operator()() = 0;
        virtual void SetData(void* data, size_t size, const std::type_info & type) = 0;
        virtual void GetData(void* data, size_t& size, std::string& type) = 0;
        virtual std::string& GetType() = 0;
    };
};

#endif
