//
//  IRender.h
//  Helios
//
//  Created by James Hurley on 9/19/11.
//

#ifndef Helios_IRender_h
#define Helios_IRender_h
#include <Render/RenderCommand.h>
#include <string>
#include <fstream>
#include <common.h>
/*
 #define GL_BYTE                           0x1400
 #define GL_UNSIGNED_BYTE                  0x1401
 #define GL_SHORT                          0x1402
 #define GL_UNSIGNED_SHORT                 0x1403
 #define GL_INT                            0x1404
 #define GL_UNSIGNED_INT                   0x1405
 #define GL_FLOAT                          0x1406
 #define GL_FIXED                          0x140C*/
namespace helios
{
    // -------------------------------------------------------------------------
    const int RenderOptions_StenciledShadowVolumes = 1;
    const int RenderOptions_SSAO                   = 1<<1;
    const int RenderOptions_HDR                    = 1<<2;
    const int RenderOptions_Blend                  = 1<<3;
    const int RenderOptions_Lighting               = 1<<4;
    const int RenderOptions_ShadowMaps             = 1<<5;
    const int RenderOptions_AntiAlias              = 1<<6;
    typedef int RenderOptions;
    // -------------------------------------------------------------------------
    
    struct VAOObj
    {
        int name;
        enum
        {
            R_BYTE = 0x1400,
            R_UBYTE,
            R_SHORT,
            R_USHORT,
            R_INT,
            R_UINT,
            R_FLOAT
        }
        datatype;
        size_t   size;
        size_t   stride;
        short    offset;
        unsigned char     normalize;
    };
    
   // -------------------------------------------------------------------------
    
    class IRender
    {
    protected: 
        
        static std::string
        LoadFileText(const std::string & name)
        {
            std::string ret, line;
            std::ifstream f(name.c_str());
            while(std::getline(f, line))
                ret += line + std::string("\n");
            return ret;
        };
        
    public:
        
        virtual ~IRender() {} ;
        virtual unsigned int  GenerateVBO(void* data, size_t stride, size_t length) = 0;
        virtual unsigned int  GenerateVAO(const std::vector<VAOObj> & data )  = 0;
        virtual unsigned int  GenerateIBO(unsigned* indices, size_t size) = 0;
        virtual unsigned int  GenerateIBO(unsigned short* indices, size_t size) = 0;
        virtual void SetDefaultFBO( unsigned int ) = 0;
        virtual void BindVBO ( unsigned int ) = 0;
        virtual void BindVAO ( unsigned int ) = 0;
        virtual void DeleteVBO ( unsigned int ) = 0;
        virtual void DeleteVAO ( unsigned int ) = 0;
        virtual void PushRenderCommand ( std::vector<RenderCommand> const & ) = 0;
        virtual void Render() =0;
        virtual void ClearRenderStack() = 0;
        virtual void SetViewport(int x, int y, int w, int h) = 0;
        virtual void Prepare(int fboWidth=1024, int fboHeight=1024) = 0;
        virtual void SetOptions(RenderOptions) = 0;
        virtual void ClearViewport(float r = 0., float g = 0., float b = 0., float a = 0.) = 0;
        virtual void SetUniform(int shader_name, UniformData_ptr ud) = 0;
        virtual unsigned LoadShader(std::string &vertex_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms,std::string defines = "") = 0;
        virtual unsigned LoadShader(std::string &vertex_file, std::string &geometry_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms, std::string defines = "") = 0;
        virtual void DeleteShader(unsigned shader) =0;
        // Create two triangles for rendering spritedata
        virtual unsigned GenerateDefaultVBO() = 0;
        virtual unsigned GenerateDefaultIBO() = 0;
        virtual void ClearRenderStages() = 0;
    };
    

}

#endif
