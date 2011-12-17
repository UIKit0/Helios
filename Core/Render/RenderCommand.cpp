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
         //   uniforms.clear();
            //uniforms.insert(uniforms.begin(), r.uniforms.begin(), r.uniforms.end());
            uniforms = r.uniforms;
            iboSize = r.iboSize;
            iboOffset = r.iboOffset;
            ibo = r.ibo;
            vbo = r.vbo;
            vao = r.vao;
            shader = r.shader;
            tex = r.tex;
            depth = r.depth;
            state = r.state;
        }
        

    }
    RenderCommand&
    RenderCommand::operator= (RenderCommand const & r)
    {
        if(this == &r)
            return *this;
        
        //uniforms.clear();
        //uniforms.reserve(r.uniforms.size());
        //uniforms = r.uniforms;
        
        iboSize = r.iboSize;
        iboOffset = r.iboOffset;
        ibo = r.ibo;
        vbo = r.vbo;
        vao = r.vao;
        shader = r.shader;
        tex = r.tex;
        depth = r.depth;
        //printf("Reserving: %zu\n", r.uniforms.size());
        uniforms = r.uniforms;
        state = r.state;
        //uniforms.insert(uniforms.begin(), r.uniforms.begin(), r.uniforms.end());
        return *this;
    };
}
