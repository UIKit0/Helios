//
//  RenderCommand.cpp
//  Helios
//
//  Created by James Hurley on 10/11/11.
//

#include <iostream>
#include <Render/RenderCommand.h>

namespace helios
{
    UniformData::UniformData(UniformData const & u)
    {
        name = u.name;
        type = u.type;
        size = u.size;
        count = u.count;
        memcpy(data, u.data, size);
    }
    UniformData& 
    UniformData::operator= (UniformData const & u)
    {
        name = u.name;
        type = u.type;
        size = u.size;
        count = u.count;
        memcpy(data, u.data, size);
        
        
        return *this;
    }
    // -------------------------------------------------------------------------
    RenderCommand::RenderCommand(RenderCommand const & r)
    {
        {
            uniforms = r.uniforms;
            iboSize = r.iboSize;
            iboOffset = r.iboOffset;
            ibo = r.ibo;
            vbo = r.vbo;
            for(int i = 0 ; i < (sizeof(tex) / sizeof(tex[0])) ; ++i)
                tex[i] = r.tex[i];
            
            depth = r.depth;
            state = r.state;
        }
        

    }
    RenderCommand&
    RenderCommand::operator= (RenderCommand const & r)
    {
        if(this == &r)
            return *this;
        
        iboSize = r.iboSize;
        iboOffset = r.iboOffset;
        ibo = r.ibo;
        vbo = r.vbo;
        for(int i = 0 ; i < (sizeof(tex) / sizeof(tex[0])) ; ++i)
            tex[i] = r.tex[i];
        depth = r.depth;
        uniforms = r.uniforms;
        state = r.state;

        return *this;
    };
}
