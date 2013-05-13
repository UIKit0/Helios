
#ifndef Helios_GL32Render_h
#define Helios_GL32Render_h

#include <Render/IRender.h>
#include <Render/ShaderManager.h>

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#define MAX_COLOR_ATTACHMENTS 4

namespace helios
{
    namespace gl32
    {
        struct FBO
        {
            GLuint colorBufferCount;
            GLuint name;
            GLuint colorBufferTex[MAX_COLOR_ATTACHMENTS];
            GLuint depthBufferTex;
            GLuint stencilBufferTex;
        };
        struct UniformPair
        {
            UniformData_ptr uniform;
            int shader_id;        
        };
        class GL32Render : public IRender
        {
            
        protected:
            
            ShaderManager mShaderManager;
            
            GLuint mDefaultVBO = 0;
            GLuint mDefaultIBO = 0 ;
            GLuint mDefaultFBO = 0;
            GLuint mDefaultVAO = 0;
            
            std::map<e::RenderStage, GLuint> mShader;
            std::map<e::RenderStage, FBO> mFBO;
            std::map<unsigned,RenderGroup> mGroups;
            std::map<unsigned, unsigned> mVAOMap;
            
            std::vector<UniformPair> mUniforms;
            
            RenderOptions mOptions;
            
            struct
            {
                short x, y, w, h;
            } mCurrentViewport;
            
            short mCurrentVBO=0, mCurrentIBO=0, mCurrentVAO=0, mCurrentTex=0, mCurrentShader=0;
            
            void GenerateFBO(e::RenderStage, size_t w, size_t h, int colorAttachments = 1);
            void RenderStage(e::RenderStage, std::vector<RenderGroup>& gVec);
            void RenderShadows(std::vector<RenderGroup>& gVec);
            void SetUniforms(std::vector<UniformData_ptr>& uniformdata, Shader& currentShader, Shader& targetShader, int clear = 0);
        public:
            
            GL32Render();
            ~GL32Render();
            unsigned GenerateDefaultVBO();
            unsigned GenerateDefaultIBO();
            unsigned int  GenerateVBO(void* data, size_t stride, size_t length) ;
            unsigned int  GenerateVAO(const std::vector<VAOObj>& data ) ;
            unsigned int  GenerateIBO(unsigned * indices, size_t size);
            unsigned int  GenerateIBO(unsigned short * indices, size_t size);
            
            void BindVBO ( unsigned int ) ;
            void BindVAO ( unsigned int ) ;
            void DeleteVAO ( unsigned int ) ;
            void DeleteVBO ( unsigned int );
            void PushRenderCommand ( std::vector<RenderCommand> const & ) ;
            void ClearViewport(float r = 0., float g = 0., float b = 0., float a = 0.);
            void Render() ;
            void Prepare( int fboWidth = 2048, int fboHeight = 2048 );
            void SetOptions( RenderOptions options ) { mOptions = options ; } ;
            void ClearRenderStack() ;
            void SetViewport(int x, int y, int w, int h) ;
            void SetDefaultFBO( unsigned int fbo )
            {
                mDefaultFBO = fbo;
            };
            void SetUniform(int shader_name, UniformData_ptr ud);
            
            unsigned LoadShader(std::string &vertex_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms, std::string defines = "");
            unsigned LoadShader(std::string &vertex_file, std::string &geometry_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms, std::string defines = "");
            
            
            void SetRenderStage(e::RenderStage stage, std::vector<UniformData> parameters);
            void ClearRenderStages() {};
            
            void DeleteShader(unsigned shader);
            
        };

    };
};

namespace helios
{
    using namespace helios::gl32;
};
#endif
