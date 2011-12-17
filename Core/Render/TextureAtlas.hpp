//
//  TextureAtlas.hpp
//  mercury
//
//  Created by James Hurley on 6/17/11.
//  Copyright 2011 5th Planet Games. All rights reserved.
//

#ifndef mercury_TextureAtlas_hpp
#define mercury_TextureAtlas_hpp

#include <Render/Texture.hpp>
#include <map>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

/**
 *  TODO: Need a y-offset for text, letters that drop below the main line
 *        such as p, g, q, etc.. appear strange without it.
 */ 
namespace helios
{

   
    
    class TextureAtlas : public Texture
    {
    private:
        boost::property_tree::ptree pt;
        std::map<std::string, frame_t> frames;

    public:
        TextureAtlas(std::string imagefile, std::string referencefile, short flags = 0)
        : Texture(imagefile, flags)
        {

            try {
            boost::property_tree::read_json(referencefile, pt);
            }
            catch(boost::property_tree::json_parser::json_parser_error &je)
            {
                std::cout << "Error parsing: " << je.filename() << " on line: " << je.line() << std::endl;
                std::cout << je.message() << std::endl;
            }
                boost::property_tree::ptree::const_iterator it, ite;
            it = pt.get_child("frames").begin();
            ite = pt.get_child("frames").end();
            
            for ( ; it != ite ; ++it )
            {
                frame_t frame;
                frame.name = mTextureId;
                frame.coords.minS = (*it).second.get_child("minS").get_value<float>()+0.001f;
                frame.coords.minT = (*it).second.get_child("minT").get_value<float>()+0.001f;
                // internally texture coordinates are stored as shorts.  Subtract 0.001f to compensate for some instability in the coordinates.
                frame.coords.maxS = (*it).second.get_child("maxS").get_value<float>()-0.001f;
                frame.coords.maxT = (*it).second.get_child("maxT").get_value<float>()-0.001f;
                frame.h = (*it).second.get_child("h").get_value<int>();
                frame.w = (*it).second.get<int>("w");
                frame.xoffset = (*it).second.get<int>("xo",0);
                frame.yoffset = (*it).second.get<int>("yo",0);
                frames[(*it).second.get_child("name").get_value<std::string>()] = frame;
            }
        };
        
        frame_t operator[] ( std::string name ) 
        {
            return frames[name];
        };
        
        frame_t GetFrame(std::string const & name ) 
        {

            return frames[name];
        }

             
    };
};

#endif
