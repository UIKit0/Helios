//
//  GLES2Render.h
//  Helios
//
//  Created by James Hurley on 9/19/11.
//

#ifndef Helios_GLES2Render_h
#define Helios_GLES2Render_h

#include <Render/IRender.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <Render/TextureAtlas.hpp>
#include <Render/Texture.hpp>
#include <Render/ShaderManager.h>

namespace helios
{
    namespace gles2
    {
        struct FBO
        {
            GLuint name;
            GLuint colorBufferTex;
            GLuint depthBufferTex;
            GLuint stencilBufferTex;
        };
        class GLES2Render : public IRender
        {
        protected:
            
            ShaderManager mShaderManager;
            
            GLuint mDefaultVBO = 0;
            GLuint mDefaultIBO = 0 ;
            GLuint mDefaultFBO = 0;
            GLuint mDiffuseProgram, mShadowVolumeProgram, mShadowMapProgram;
            
            std::map<e::RenderStage, FBO> mFBO;
            // std::vector<RenderCommand> mStack;
            std::map<unsigned,RenderGroup> mGroups;
            
            RenderOptions mOptions;
            
            struct
            {
                short x, y, w, h;
            } mCurrentViewport;
            
            short mCurrentVBO=0, mCurrentIBO=0, mCurrentVAO=0, mCurrentTex=0, mCurrentShader=0;
            
            void GenerateFBO(e::RenderStage, size_t w, size_t h);
            void RenderStage(e::RenderStage, std::vector<RenderGroup>& gVec);
            void RenderStencilShadows(std::vector<RenderGroup>& gVec);
            void RenderShadowMaps(std::vector<RenderGroup>& gVec);
            void SetUniforms(std::vector<UniformData_ptr>& uniformdata, Shader& currentShader, Shader& targetShader, int clear = 0);
        public:
            
            GLES2Render();
            ~GLES2Render();
            unsigned GenerateDefaultVBO();
            unsigned GenerateDefaultIBO();
            unsigned int  GenerateVBO(void* data, size_t stride, size_t length) ;
            unsigned int  GenerateVAO(std::vector<VAOObj> data, unsigned vbo ) ;
            unsigned int  GenerateIBO(unsigned * indices, size_t size);
            unsigned int  GenerateIBO(unsigned short * indices, size_t size);
            
            void BindVBO ( unsigned int ) ;
            void BindVAO ( unsigned int ) ;
            void DeleteVAO ( unsigned int ) ;
            void DeleteVBO ( unsigned int );
            void PushRenderCommand ( std::vector<RenderCommand> const & ) ;
            void ClearViewport(float r = 0., float g = 0., float b = 0., float a = 0.);
            void Render() ;
            void Prepare(int fboWidth=1024, int fboHeight=1024);
            void SetOptions( RenderOptions options ) { mOptions = options ; } ;
            void ClearRenderStack() ;
            void SetViewport(int x, int y, int w, int h) ;
            void SetDefaultFBO( unsigned int fbo )
            {
                mDefaultFBO = fbo;
            };
            unsigned LoadShader(std::string &vertex_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms,std::string defines = "");
            unsigned LoadShader(std::string &vertex_file, std::string &geometry_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms,std::string defines = "")
            {
                // GLES2 doesn't support geometry shaders
                return LoadShader(vertex_file, fragment_file, attribs, uniforms);
            };
            
            void SetUniform(int shader_name, UniformData_ptr ud) {};
            void SetRenderStage(e::RenderStage stage, std::vector<UniformData> parameters);
            void ClearRenderStages() {};
            
            void DeleteShader(unsigned shader);
        };  
    };
    
};

namespace helios
{
    using namespace helios::gles2;
};

#endif
