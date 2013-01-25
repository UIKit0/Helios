//
//  Configuration.cpp
//  Helios
//
//  Created by James Hurley on 1/25/13.
//  Copyright (c) 2013 James Hurley. All rights reserved.
//

#include <World/Configuration.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>

namespace helios
{
    Configuration::Configuration(const std::string file)
    {
        boost::property_tree::ptree pt;

        try
        {
            boost::property_tree::read_json(file, pt);
        }
        catch(boost::property_tree::json_parser::json_parser_error &je)
        {
            std::cout << "Error parsing: " << je.filename() << " on line: " << je.line() << std::endl;
            std::cout << je.message() << std::endl;
        }
        boost::property_tree::ptree::const_iterator it, ite ;

 
        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child("keybinding"))
        {

            char key = v.second.get_value<int>() ;
            std::string value = v.first.data() ;

            std::cout << "Found (" << v.first.data() << "," << v.second.get_value<int>() << ")" << std::endl ;

            mKeybindings[key] = value ;

        }
    }
    Configuration::~Configuration()
    {
        
    }
}