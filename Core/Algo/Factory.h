//
//  Factory.h
//  Helios
//  jamesghurley<at>gmail.com

//  Generic Factory pattern implementation
//  See: https://bitbucket.org/kniht/scraps/src/6f5692c4f9d2/cpp/kniht/simple_factory.hpp
//

#ifndef Helios_Factory_h
#define Helios_Factory_h
#include <memory>

namespace helios
{
    template<typename Interface, typename KeyT=std::string>
    struct Factory
    {
        
        typedef KeyT Key;
        typedef std::auto_ptr<Interface> Type;
        typedef Type (*Creator)();
        
        bool define(Key const& key, Creator v) 
        {
            return _registry.insert(typename Registry::value_type(key, v)).second;
        }
        Type create(Key const & key) {
            
            auto i = _registry.find(key);
            
            if ( i == _registry.end() )
            {
                throw std::invalid_argument(std::string(__PRETTY_FUNCTION__) + ": key not registered");
            }
            else return i->second();
        }
        
        template<typename Base, typename Actual>
        static
        std::auto_ptr<Base> create_func() 
        {
            return std::auto_ptr<Base>(new Actual());
        }
                                    
    private:
        typedef std::map<Key, Creator> Registry;
        Registry _registry;
                                    
    };
}

#endif
