//
//  GLES2Render.cpp
//  Helios
//
//  Created by James Hurley on 9/19/11.
//

#include <iostream>
#include <Render/GLES2/GLES2Render.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <world/SceneManager.h>
#include <Render/Vertex.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#ifdef DEBUG
#define eglError( Error )\
{\
D_PRINT("OpenGL Error: %s", Error );\
\
assert( 0 );\
}

#define eglGetError( )\
{\
for ( GLenum Error = glGetError( ); ( GL_NO_ERROR != Error ); Error = glGetError( ) )\
{\
switch ( Error )\
{\
case GL_INVALID_ENUM:      eglError( "GL_INVALID_ENUM"      ); break;\
case GL_INVALID_VALUE:     eglError( "GL_INVALID_VALUE"     ); break;\
case GL_INVALID_OPERATION: eglError( "GL_INVALID_OPERATION" ); break;\
case GL_OUT_OF_MEMORY:     eglError( "GL_OUT_OF_MEMORY"     ); break;\
default:                                                        break;\
}\
}\
}

#define eglCheckFramebufferStatus( )\
{\
switch ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) )\
{\
case GL_FRAMEBUFFER_COMPLETE: printf("Framebuffer COMPLETE\n"); break;\
case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"         ); assert( 0 ); break;\
case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" ); assert( 0 ); break;\
case GL_FRAMEBUFFER_UNSUPPORTED:                   printf( "\n%s\n\n", "GL_FRAMEBUFFER_UNSUPPORTED"                   ); assert( 0 ); break;\
default:                                                                                                                              break;\
}\
}

#else
#define eglGetError()
#define eglCheckFramebufferStatus()
#endif


#define FBO_WIDTH  1024
#define FBO_HEIGHT 1024
namespace helios
{
    namespace gles2
    {
        
    
    inline void 
    getProgramLog(GLuint program)
    {
        GLint length;
        GLint status;
        
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char log[length];
        glGetProgramInfoLog(program, length, &length, log);
        D_PRINT("Program Log( %d ): %s\n---\n", status, log);
    }
    inline void 
    getShaderLog(GLuint shader)
    {
        GLint length;
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char log[length];
        glGetShaderInfoLog(shader,length,&length,log);
        D_PRINT("Log ( %d ): %s\n---\n", status, log);
    }
    
    GLES2Render::GLES2Render()
    {
        
    }
    GLES2Render::~GLES2Render()
    {
        if(mDefaultVBO)
        {
            glDeleteBuffers(1, &mDefaultVBO);
        }
    }
    void 
    GLES2Render::Prepare(int fboWidth, int fboHeight)
    {
        
        if( mOptions & RenderOptions_StenciledShadowVolumes ) {
            
            std::string b_folder = SceneManager::Inst().GetResourceFolder();
            std::string v = b_folder + "/Diffuse.vsh";
            std::string f = b_folder + "/Diffuse.fsh";
            std::map<std::string, int> attribs;
            std::map<std::string, int> uniforms;
        
            attribs[e::kVertexAttribPosition] = e::kVertexAttribPositionPosition;
            attribs[e::kVertexAttribNormal] = e::kVertexAttribPositionNormal;
            attribs[e::kVertexAttribDiffuseColor]  = e::kVertexAttribPositionDiffuseColor;
            attribs[e::kVertexAttribTexCoord] = e::kVertexAttribPositionTexCoord;
            attribs[e::kVertexAttribBoneId] = e::kVertexAttribPositionBoneId;
          //  attribs[e::kVertexAttribNoExtrude] = e::kVertexAttribPositionNoExtrude;
        
            uniforms[e::kVertexUniformModelView] = 0;
            uniforms[e::kVertexUniformProjection] = 0;
            uniforms[e::kVertexUniformNormalMat] = 0;
            uniforms[e::kVertexUniformLightPos] = 0;
            uniforms[e::kFragmentUniformSampler0] = 0;
            uniforms[e::kVertexUniformJoints] = 0;
        
            mDiffuseProgram = LoadShader(v, f, attribs, uniforms);
            v = b_folder + "/ShadowVolume.vsh";
            f = b_folder + "/ShadowVolume.fsh";
            mShadowVolumeProgram = LoadShader(v,f, attribs,uniforms);
            glEnable(GL_STENCIL_TEST);
        }
        /** Shadow Maps **/
        if( mOptions & RenderOptions_ShadowMaps ) {
            GenerateFBO(e::kRenderStageShadows, fboWidth, fboHeight);
            std::string b_folder = SceneManager::Inst().GetResourceFolder();
            std::string v = b_folder + "/ShadowMap.vsh";
            std::string f = b_folder + "/ShadowMap.fsh";
            std::map<std::string, int> attribs;
            std::map<std::string, int> uniforms;
            
            attribs[e::kVertexAttribPosition] = e::kVertexAttribPositionPosition;
            //attribs[e::kVertexAttribNormal] = e::kVertexAttribPositionNormal;
            //attribs[e::kVertexAttribDiffuseColor]  = e::kVertexAttribPositionDiffuseColor;
            attribs[e::kVertexAttribTexCoord] = e::kVertexAttribPositionTexCoord;
            attribs[e::kVertexAttribBoneId] = e::kVertexAttribPositionBoneId;
            
            uniforms[e::kVertexUniformTexMat] = 0;
            uniforms[e::kVertexUniformModelView] = 0;
            uniforms[e::kVertexUniformProjection] = 0;
            //uniforms[e::kVertexUniformNormalMat] = 0;
           // uniforms[e::kVertexUniformLightPos] = 0;
            uniforms[e::kFragmentUniformSampler0] = 0;
            uniforms[e::kVertexUniformJoints] = 0;
            
            mShadowMapProgram = LoadShader(v, f, attribs, uniforms);
        }
        
        std::string ext = std::string((const char *)glGetString(GL_EXTENSIONS));
        
       
        
        if( mOptions & RenderOptions_Blend ) {
            glEnable(GL_BLEND);            
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        
#ifdef DEBUG
      
        GLint maxFUniforms=0, maxVUniforms, maxVaryings, maxAttribs;
        glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryings);
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFUniforms);
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,&maxVUniforms);
        D_PRINT("Implementation info: \n\tmaxFUniforms: %d VUniforms: %d\n\tmaxVaryings: %d\n\tmaxAttribs: %d", maxFUniforms, maxVUniforms, maxVaryings, maxAttribs);
        D_PRINT("Available Extensions: \n\t%s", glGetString(GL_EXTENSIONS)); 
#endif
        glClearColor(0.f,0.f,0.f,0.f);
    }
    unsigned 
    GLES2Render::GenerateDefaultIBO()
    {
        if ( !mDefaultIBO )
        {
            unsigned short indices [6] = {  0 , 1 , 2, 1, 3, 2  } ;
            
            mDefaultIBO = GenerateIBO(indices, 6);
        }
        return mDefaultIBO;
        
    }
    unsigned 
    GLES2Render::GenerateDefaultVBO()
    {
        if ( !mDefaultVBO ) 
        {
            Vertex v [4];
            unsigned short minS = 0;
            unsigned short maxS = 0xFFFF;
            unsigned short minT = 0;
            unsigned short maxT = 0xFFFF;
            v [0] = Vertex( { glm::vec4(-0.5f,-0.5f,0.f,1.f),minS,maxT ,0 ,0, 0, 0,0,0} );
            
            v [1] = Vertex( { glm::vec4(0.5f,-0.5f,0.f,1.f), maxS,maxT ,0 ,0, 0, 0,0,0 } );
            v [2] = Vertex( { glm::vec4(-0.5f,0.5f,0.f,1.f), minS,minT ,0 ,0, 0, 0,0,0} );
            
            v [3] = Vertex( { glm::vec4(0.5f,0.5f,0.f,1.f), maxS,minT ,0 ,0, 0, 0,0,0 } );

            
            mDefaultVBO = GenerateVBO(&v, sizeof(v), sizeof(v) * 4);
        }
        return mDefaultVBO;
    }
    
    void 
    GLES2Render::ClearViewport(float r , float g, float b , float a )
    {
        glClearColor(r,g,b,a);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) ;
        
    }
    unsigned int  
    GLES2Render::GenerateVBO(void* data, size_t stride, size_t length) 
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);
        eglGetError();
        return vbo;
    }
    unsigned int  
    GLES2Render::GenerateVAO(std::vector<VAOObj> data, unsigned vbo )
    {
        GLuint vao;
        glGenVertexArraysOES(1, &vao);
        glBindVertexArrayOES(vao);

        auto it = data.begin();
        auto ite = data.end();
        for ( ; it != ite ; ++ it )
        {
            if(it->name >= 0) {
                glEnableVertexAttribArray((*it).name);
                glVertexAttribPointer((*it).name, (*it).size, (*it).datatype, ((*it).normalize ? GL_TRUE : GL_FALSE), (*it).stride, BUFFER_OFFSET((*it).offset));
            }
        }
        eglGetError();
        
        glBindVertexArrayOES(0);
        
        return vao;
    }
    unsigned int  
    GLES2Render::GenerateIBO(unsigned* indices, size_t size)
    {
        assert("This platform does not support 32-bit indices.");
        return 0;
    }
    unsigned int  
    GLES2Render::GenerateIBO(unsigned short* indices, size_t size)
    {
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(short), &indices[0], GL_STATIC_DRAW);
        eglGetError();
        return ibo;
    }
    void 
    GLES2Render::BindVBO ( unsigned int vbo ) 
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        eglGetError();
    }
    void 
    GLES2Render::BindVAO ( unsigned int vao ) 
    {
        glBindVertexArrayOES ( vao ) ;
        eglGetError();
    }
    void 
    GLES2Render::DeleteVBO ( unsigned int vbo )
    {
        glDeleteBuffers(1, &vbo);
        eglGetError();
    }
    void
    GLES2Render::DeleteVAO ( unsigned int vao ) 
    {
        glDeleteVertexArraysOES(1, &vao);
        eglGetError();
    }
    void 
    GLES2Render::PushRenderCommand ( std::vector<RenderCommand> const & r) 
    {
        for ( auto it = r.begin() ; it != r.end() ; ++it ) {
            // Skip this command if it has no geometry associated with it
            if((*it).iboSize == 0) 
                continue;
            unsigned hash = RenderGroup::Hash((*it));
            mGroups[hash].commands.push_back((*it));
            mGroups[hash].vbo = (*it).vbo;
            mGroups[hash].ibo = (*it).ibo;
            mGroups[hash].vao = (*it).vao;
            mGroups[hash].shader = (*it).shader;
            mGroups[hash].tex = (*it).tex;
            mGroups[hash].hash = hash;
        }
    }
    void 
    GLES2Render::SetUniforms(std::vector<UniformData_ptr>& uniformdata, Shader& currentShader, Shader& targetShader, int clear)
    {
        for ( auto jt = uniformdata.begin() ; jt != uniformdata.end() ; ++jt ) 
        {
            UniformData ud = (*(*jt));
            int name = ud.name;
            
            if( currentShader.name != targetShader.name )
            {
                name = currentShader.GetRemappedUniform(targetShader, name);
                if(name < 0) continue;
            }
            switch(ud.type)
            {
                case UniformData::kUTFloat:
                {
                    glUniform1fv(name, ud.count, ud.GetData<GLfloat>());
                    eglGetError();
                }
                    break;
                case UniformData::kUTInt:
                {
                    glUniform1iv(name, ud.count, ud.GetData<GLint>());
                    eglGetError();
                }
                    break;
                case UniformData::kUTVec3:
                {
                    glUniform3fv(name, ud.count, ud.GetData<GLfloat>());
                    eglGetError();
                }
                    break;
                case UniformData::kUTVec4:
                {
                    glUniform4fv(name, ud.count, ud.GetData<GLfloat>());
                    eglGetError();
                }
                    break;
                case UniformData::kUTMat3:
                {
                    glUniformMatrix3fv(name,ud.count,GL_FALSE,ud.GetData<GLfloat>());
                    eglGetError();
                }
                    break;
                case UniformData::kUTMat4:
                {
                    glUniformMatrix4fv(name,ud.count,GL_FALSE, ud.GetData<GLfloat>());
                    eglGetError();
                }
                    break;

            }
        }

    }
    void
    GLES2Render::RenderShadowMaps(std::vector<RenderGroup>& gVec)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO[e::kRenderStageShadows].name);
       // glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glViewport(mCurrentViewport.x, mCurrentViewport.y, mCurrentViewport.w,mCurrentViewport.h);
        glScissor(mCurrentViewport.x, mCurrentViewport.y, mCurrentViewport.w,mCurrentViewport.h);
        mCurrentShader = mShadowMapProgram;
        glUseProgram(mCurrentShader);
        RenderStage(e::kRenderStageShadows, gVec);
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, mFBO[e::kRenderStageShadows].colorBufferTex);
        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFBO);
    }
    void
    GLES2Render::RenderStencilShadows(std::vector<RenderGroup>& gVec)
    {
        
        glDisable(GL_BLEND);
        glUseProgram(mDiffuseProgram);
        mCurrentShader = mDiffuseProgram;

        RenderStage(e::kRenderStageDiffuse, gVec);
        
        
        glClear(GL_STENCIL_BUFFER_BIT);
       
        glStencilFunc(GL_ALWAYS, 0, ~0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_GREATER);
        
        glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
        glDepthMask(GL_FALSE);
        glUseProgram(mShadowVolumeProgram);
        mCurrentShader = mShadowVolumeProgram;
        
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_BACK , GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        
        RenderStage(e::kRenderStageShadows, gVec);
        
        glDepthFunc(GL_LEQUAL);
  
        glStencilFunc(GL_EQUAL, 0, ~0);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glUseProgram(mDiffuseProgram);
        mCurrentShader = mDiffuseProgram;
        
        
        RenderStage(e::kRenderStageDiffuse,gVec);  
        
        
        glDepthFunc(GL_LESS);
        glStencilFunc(GL_ALWAYS, 0, ~0);
    }
    void 
    GLES2Render::RenderStage(e::RenderStage stage,  std::vector<RenderGroup>& gVec)
    {
        
        Shader currentShader;
        Shader targetShader ;
        GLuint tex = 0;//mCurrentTex;
        GLuint vbo = 0;//mCurrentVBO; 
        GLuint vao = 0;//mCurrentVAO;
        GLuint ibo = 0;//mCurrentIBO;
        
        GLuint shader = 0;//mCurrentShader;
        
        
        auto it = gVec.begin();
        auto ite = gVec.end();
        
        for ( ; it != ite ; ++it )
        {
            std::sort((*it).commands.begin(), (*it).commands.end());
            
            if(tex != (*it).tex) {
                tex = (*it).tex;
                glBindTexture(GL_TEXTURE_2D, tex);
                eglGetError();
            }
            if(vbo != (*it).vbo) {
                vbo = (*it).vbo;
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                
            }
            if(vao != (*it).vao)
            {
                vao = (*it).vao;
                glBindVertexArrayOES(vao);
                eglGetError();
            }
            if(ibo != (*it).ibo)
            {
                ibo = (*it).ibo;
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                eglGetError();
            }
            
            if( shader != (*it).shader && (*it).shader > -1) {
                if(stage == e::kRenderStageGeometry) {
                    glUseProgram((*it).shader);
                    shader = (*it).shader;
                
                    currentShader = targetShader = mShaderManager.GetShader(shader);
                } else {
                    currentShader = mShaderManager.GetShader(mCurrentShader);
                    targetShader = mShaderManager.GetShader((*it).shader);
                }
                eglGetError();
            } 
            
            
             //TODO: Replace with light component.
             /*{
                int lp = currentShader.GetUniform(e::kVertexUniformLightPos);
               
                
                if(lp > -1)
                    glUniform4f(lp, 35.f ,40.f,35.0f,1.f);
            
                
                eglGetError();
            }*/
            for ( auto jt = (*it).commands.begin() ; jt != (*it).commands.end() ; ++jt) {
                
                
                if((stage == e::kRenderStageShadows && (*jt).state.shadowmap) || stage != e::kRenderStageShadows) {
                
                    SetUniforms((*jt).uniforms,currentShader,targetShader);
                    eglGetError();
                    glDrawElements(GL_TRIANGLES, (*jt).iboSize , GL_UNSIGNED_SHORT, (void*)((*jt).iboOffset*sizeof(short)));
                
                    eglGetError();
                
                }
            }
        }
        

    }
    
    void 
    GLES2Render::Render() 
    {
        
        std::vector<RenderGroup> gVec;
        std::transform(mGroups.begin(), mGroups.end(), std::back_inserter(gVec), boost::bind(&std::map<unsigned,RenderGroup>::value_type::second,_1));
        
        if ( mOptions & RenderOptions_Blend ) {
            
#ifdef _LIBCPP_STABLE_APPLE_ABI
            std::sort<helios::RenderGroup>(gVec.begin(),gVec.end());
#else
            std::sort(gVec.begin(), gVec.end());
#endif
        }
        
        if ( mOptions & RenderOptions_ShadowMaps )
            RenderShadowMaps(gVec);
        
        if( mOptions & RenderOptions_StenciledShadowVolumes) 
            RenderStencilShadows(gVec);
        
        else {
         
            RenderStage(e::kRenderStageGeometry,gVec);
        }
        ClearRenderStack();
    }
    void 
    GLES2Render::ClearRenderStack() 
    {
        mGroups.clear();

    }
    void 
    GLES2Render::SetViewport(int x, int y, int w, int h) 
    {
        if( mCurrentViewport.x != x || mCurrentViewport.y != y || mCurrentViewport.w != w || mCurrentViewport.h != h){ 
            glViewport(x,y,w,h);
            glScissor(x,y,w,h);
            mCurrentViewport.x = x;
            mCurrentViewport.y = y;
            mCurrentViewport.w = w;
            mCurrentViewport.h = h;
        }
    }
    unsigned 
    GLES2Render::LoadShader(std::string & vertex_file, std::string & fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms, std::string defines)
    {
        if(mShaderManager.ShaderLoaded(vertex_file + fragment_file + defines, uniforms)) {
            mShaderManager.RetainShader( mShaderManager.GetShader(vertex_file + fragment_file + defines));
            return mShaderManager.GetShader(vertex_file + fragment_file + defines).name;
        }
        D_PRINT("Generating New Shader for %s", vertex_file.c_str());
        std::string vs = IRender::LoadFileText(vertex_file);
        std::string fs = IRender::LoadFileText(fragment_file);
        
       // D_PRINT("%s: %s", vertex_file.c_str(), vs.c_str());
        GLuint vshad = glCreateShader(GL_VERTEX_SHADER);
        GLuint fshad = glCreateShader(GL_FRAGMENT_SHADER);
        GLuint prog = glCreateProgram();
        
        const char * cstr [] = { defines.c_str(), vs.c_str() };
        eglGetError();
        glShaderSource(vshad, 2, cstr, NULL);
        glCompileShader(vshad);
        getShaderLog(vshad);
        glAttachShader(prog, vshad);
        cstr[1] = fs.c_str();
        eglGetError();
        glShaderSource(fshad, 2, cstr, NULL);
        
        eglGetError();
        glCompileShader(fshad);
        eglGetError();
#ifdef DEBUG
        
        getShaderLog(fshad);
        
#endif
        
        glAttachShader(prog, fshad);
        
        auto it = attribs.begin();
        auto ite = attribs.end();
        
        for ( ; it != ite ; ++it )
        {
            const char * str = (*it).first.c_str();

            glBindAttribLocation(prog, (*it).second, str);

        }
        
        glLinkProgram(prog);
#ifdef DEBUG
        getProgramLog(prog);
#endif
        eglGetError();
        glUseProgram(prog);
        
        eglGetError();

        it = uniforms.begin();
        ite = uniforms.end();
        
        for ( ; it != ite ; ++ it )
        {
            const char * str = (*it).first.c_str();
            int i = glGetUniformLocation(prog, str);
           
            if ( i < 0 ) {
                D_PRINT("Uniform '%s' not found %d!\n", str, i);
            }
            
            (*it).second = i;
        }
        
        Shader s ;
        s.name = prog;
        s.filename = vertex_file + fragment_file + defines;
        s.uniforms = uniforms;
        s.attribs = attribs;
        
        mShaderManager.AddShader(s);
        
        eglGetError();
        return prog;                
    }
    void
    GLES2Render::DeleteShader(unsigned int shader)
    {
        if(mShaderManager.RemoveShader(mShaderManager.GetShader(shader)))
        {   
            //glDeleteShader(shader);
        }
    }
    
    void
    GLES2Render::GenerateFBO(e::RenderStage r, size_t w, size_t h)
    {
        GLuint fbo;
       
        GLuint tex;
        GLuint rbo;
        glGenRenderbuffers(1, &rbo);
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glGenTextures(1, &tex);
        
        
        glBindTexture(GL_TEXTURE_2D, tex);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w,h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
        
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        
        /*glBindTexture(GL_TEXTURE_2D, tex[1]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mCurrentViewport.w, mCurrentViewport.h, 0, GL_DEPTH_COMPONENT,GL_UNSIGNED_INT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex[1], 0);
       */
       /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mCurrentViewport.w, mCurrentViewport.h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex[2], 0);*/
        
        FBO f;
        f.name = fbo;
        f.colorBufferTex = tex;
        //f.depthBufferTex = tex[1];
        //f.stencilBufferTex = tex[2];
        
        //mFBO.push_back(f);
        mFBO[r] = f;
        eglCheckFramebufferStatus();
        glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFBO);
       // return fbo;
    }
    void
    GLES2Render::SetRenderStage(e::RenderStage stage, std::vector<UniformData> parameters)
    {
        if(mFBO.find(stage) != mFBO.end())
        {
            glDeleteFramebuffers(1, &mFBO[stage].name);
        }
        GenerateFBO(stage,FBO_WIDTH,FBO_HEIGHT);
        
    }
    }
}
