/**
 *
 *  Basic event container.  Has a target, a name, and a payload of a specified data type.  Data types must 
 *  have an overloaded assignment operator if they aren't trivial to copy.
 *  
 *  jamesghurley<at>gmail.com
 *
 **/

#ifndef Helios_Event_h
#define Helios_Event_h

#include <string>
#include <boost/shared_ptr.hpp>

namespace helios
{     
    class IEntity;
    
    class IEvent
    {
    public:
        virtual ~IEvent() {};
        virtual std::string& GetName() = 0;
        virtual std::string& GetTarget() = 0;
        virtual void SetName(std::string const & name) = 0;
        virtual void SetTarget(std::string const & target) = 0;
        virtual void SetOwner(IEntity* owner) = 0;
        virtual IEntity* GetOwner() = 0;
    };
    // -------------------------------------------------------------------------
    typedef std::shared_ptr<IEvent> IEvent_ptr;
    // -------------------------------------------------------------------------
    
    template<typename T>
    class HEvent : public IEvent
    {
    private:
        T _data;
        
        std::string _name;
        std::string _target;
        IEntity*    _owner;
        
    public:
        
        void SetData(T& data)
        {
            _data = data;
        };
        static T& GetData(IEvent_ptr e)
        {
            std::shared_ptr<HEvent< T > > p = std::static_pointer_cast<HEvent<T> >(e);
            
            return (*p).GetData();
        };
        static HEvent<T> & GetEvent(IEvent_ptr e)
        {
            std::shared_ptr<HEvent< T > > p = std::static_pointer_cast<HEvent<T> >(e);
            return *p;
        };
        T& GetData()
        {
            return _data;
        };
        
        std::shared_ptr<HEvent<T> >  ptr() 
        {
            return std::shared_ptr<HEvent<T> >(new HEvent<T>(*this));
        };
        HEvent() {};
        ~HEvent() {};
        HEvent( std::string const & target, std::string const & name, T & data) : _target(target), _name(name), _data(data)
        {
           // _target = target;
           // _name = name;
           // _data = data;
        };
      
        HEvent<T>& operator= (HEvent<T> const & rhs)
        {
            _name = rhs._name;
            _target = rhs._target;
            _data = rhs._data;
            
            return *this;
        };
        
        HEvent(HEvent<T> const & rhs)
        {
            _name = rhs._name;
            _target = rhs._target;
            _data = rhs._data;
        };
        
        std::string& GetName() { return _name; };
        //----------------------------------------------------------------------
        void SetName(std::string const & name) { _name = name; } ;
        //----------------------------------------------------------------------
        std::string& GetTarget() { return _target; };
        //----------------------------------------------------------------------
        void SetTarget(std::string const & target) { _target = target; };
        //----------------------------------------------------------------------
        void SetOwner(IEntity* owner) { _owner = owner; };
        //----------------------------------------------------------------------
        IEntity* GetOwner() { return _owner; };
    };
};

#endif
