//
//  Texture.hpp
//  mercury
//
//  Created by James Hurley on 6/17/11.
//  Copyright 2011 5th Planet Games. All rights reserved.
//

#ifndef mercury_Texture_hpp
#define mercury_Texture_hpp

#include <string>
#include "SOIL.h"

#if defined(__APPLE__) || defined(__APPLE_CC__)
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
    #include <OpenGLES/ES1/gl.h>
    #include <OpenGLES/ES2/gl.h>
#else
    #include <OpenGL/gl.h>
#endif

#include <dispatch/dispatch.h>

#endif

namespace helios
{
    struct coords_t
    {
        float minS, minT, maxS, maxT;
        
    } ;
    
    
    struct frame_t
    {
        unsigned name;
        struct
        {
            float minS, minT, maxS, maxT;
        } coords;
        int w, h;
        int xoffset, yoffset;
        
    } ;
    
    class Texture
    {
    public:
        
        Texture(std::string filename, short flags = 0)
        {
            //short flags;
            //flags |= SOIL_FLAG_POWER_OF_TWO;
           
            mTextureId = SOIL_load_OGL_texture(filename.c_str(), 
                                               SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
                                               flags, &mWidth, &mHeight);  
        };
        ~Texture()
        {

            glDeleteTextures(1,&mTextureId);

        };
        int id() const { return mTextureId; } ;
        int operator()() const { return mTextureId; };
        int w() const { return mWidth; };
        int h() const { return mHeight; };
        
    protected:
        int mWidth, mHeight;
        GLuint mTextureId;
        
    };
};

#endif
