//
//  Configuration.h
//  Helios
//
//  Created by James Hurley on 1/25/13.
//  Copyright (c) 2013 James Hurley. All rights reserved.
//

#ifndef __Helios__Configuration__
#define __Helios__Configuration__

#include <common.h>
#include <map>
#include <string>

namespace helios
{
    class Configuration
    {
    private:
        
        std::map<short, std::string> mKeybindings;
        
    public:
        
        Configuration(const std::string file);
        ~Configuration();

        const std::string & GetActionForKey(short key) const
        {
            if(mKeybindings.find(key) != mKeybindings.end() )
                return mKeybindings.at(key);
            return e::kActionNotFound;
        };
    };
};

#endif /* defined(__Helios__Configuration__) */
