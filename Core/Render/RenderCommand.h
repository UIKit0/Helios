//
//  RenderCommand.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_RenderCommand_h
#define Helios_RenderCommand_h

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <boost/shared_ptr.hpp>

namespace helios
{
    struct RenderState
    {
        uint32_t shadowmap : 1; 
        uint32_t stencilshadows : 1;
    };
    struct UniformData
    {
        
        enum UniformType
        {
            kUTFloat,
            kUTInt, 
            kUTVec3,
            kUTVec4,
            kUTMat3,
            kUTMat4,

        } type;
        
        int name;
        
        union {
            float* ptr[(sizeof(float)*16)/sizeof(float*)];
            float data[16];
        };
        size_t size;
        size_t count;
        
        //----------------------------------------------------------------------
        template <typename T>
        void SetData(T& _data)
        {
            count = 1;
            size = sizeof(T);
            memcpy(data, &_data, sizeof (T));
        };
        template <typename T>
        void SetData(T _data, size_t _count)
        {
            count = _count;
            size = sizeof(T);
            memcpy(data, &_data, sizeof(T));
        };
        //----------------------------------------------------------------------
        template <typename T>
        T* GetData()
        {
            if(count==1)
                return (T*)(data);
            else
                return (T*)(ptr[0]);
        };
        //----------------------------------------------------------------------
        UniformData& operator= (UniformData const & u);
        //----------------------------------------------------------------------
        UniformData() :  count(0) { memset(data,0,sizeof(data)); };
        //----------------------------------------------------------------------
        UniformData(UniformType t, int n)  
        {
            memset(data,0,sizeof(data));
            type = t; 
            name = n ;
        };
        //----------------------------------------------------------------------
        UniformData(UniformData const & u);
        
        //----------------------------------------------------------------------
        ~UniformData() { };
    };
    typedef boost::shared_ptr<UniformData> UniformData_ptr;
    
    struct HRenderCommand
    {
        std::vector<UniformData_ptr> uniforms;
        RenderState    state;
        struct {
            unsigned short iboSize;
            unsigned short iboOffset;
        };
        unsigned short depth;
        bool operator<(HRenderCommand const & rhs) const { return depth < rhs.depth; } ;
    };
    struct RenderCommand : HRenderCommand
    {
        RenderCommand() {};
        
        unsigned short ibo;
        unsigned short vbo;
        unsigned short vao;
        short          shader;
        unsigned short tex;
        
        bool operator<(RenderCommand rhs) { return depth < rhs.depth; };
        
        RenderCommand(RenderCommand const & r);
        RenderCommand& operator=(RenderCommand const & r);
    };
    
    struct RenderGroup
    {
        std::vector<HRenderCommand> commands;
        unsigned short ibo;
        unsigned short vbo;
        unsigned short vao;
        short          shader;
        unsigned short tex;
        
        unsigned hash;
        
        unsigned short Depth() const
        {
            size_t d = 0 ;
            for ( auto it = commands.begin() ; it != commands.end() ; ++it ) 
            {
                d += (*it).depth;
            }
            d /= commands.size();
            
            return uint16_t(d);
        };
        static unsigned Hash(RenderCommand const & r)
        {
            unsigned h = 
            
            (r.ibo ^ 2) + (r.vbo ^ 3) + (r.vao ^ 5) + (r.shader ^ 7) + (r.tex ^ 11) + (( r.depth >> 7 )^ 13);
            
            return h;
        };
        bool operator<(RenderGroup const & rhs) const { 

            return Depth() < rhs.Depth();
            
        };
    };
    
    struct RenderCommand2D : RenderCommand
    {
        
    };
    
    struct RenderCommand3D : RenderCommand
    {
   //     glm::quat quat;
    };
};

#endif
