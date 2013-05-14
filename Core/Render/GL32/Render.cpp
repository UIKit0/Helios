
#include <iostream>
#include <Render/GL32/Render.h>
#include <World/SceneManager.h>
#include <Render/Vertex.h>
#include <Render/RenderCommand.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#ifdef DEBUG
#define eglError( Error )\
{\
D_PRINT("OpenGL Error: %s", Error ); }
//assert( 0 );\
//}

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

#else
#define eglGetError()
#endif

namespace helios
{
    namespace gl32 {
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
        
        GL32Render::GL32Render()
        {
            
        }
        GL32Render::~GL32Render()
        {
            if(mDefaultVBO)
            {
                DeleteVBO(mDefaultVBO);
            }
            if(mDefaultIBO)
            {
                glDeleteBuffers(1, &mDefaultIBO);
            }
            for( auto it = mShader.begin(); it != mShader.end(); ++it)
            {
                DeleteShader((*it).second);
            }
        }
        void
        GL32Render::Prepare( int fboWidth, int fboHeight )
        {
            eglGetError();
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
                
                uniforms[e::kVertexUniformModelView] = 0;
                uniforms[e::kVertexUniformProjection] = 0;
                uniforms[e::kVertexUniformNormalMat] = 0;
                uniforms[e::kVertexUniformJoints] = 0;
                uniforms[e::kVertexUniformLightPos] = 0;
                uniforms[e::kFragmentUniformSampler0] = 0;
                uniforms[e::kFragmentUniformSampler1] = 0;
                uniforms[e::kFragmentUniformSampler2] = 0;
                uniforms[e::kFragmentUniformSampler3] = 0;
                uniforms[e::kFragmentUniformSampler4] = 0;
                uniforms[e::kFragmentUniformSampler5] = 0;
                uniforms[e::kFragmentUniformSampler6] = 0;                
                
                mShader[e::kRenderStageGeometry] = LoadShader(v, f, attribs, uniforms);
                v = b_folder + "/ShadowVolume.vsh";
                f = b_folder + "/ShadowVolume.fsh";
                mShader[e::kRenderStageStencilShadows] = LoadShader(v,f, attribs,uniforms);
                v = b_folder + "/ShadowFill.vsh";
                f = b_folder + "/ShadowFill.fsh";
                mShader[e::kRenderStageShadowFill] = LoadShader(v,f,attribs,uniforms);
                glEnable(GL_STENCIL_TEST);
                glEnable(GL_DEPTH_CLAMP);
                
            }

            
            if( mOptions & RenderOptions_Blend ) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            }
            
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_DEPTH_TEST);

            glClearColor(.3f,.5f,.7f,1.f);
            eglGetError();
        }
        unsigned
        GL32Render::GenerateDefaultIBO()
        {
            if ( !mDefaultIBO )
            {
                unsigned short indices [6] = {  0 , 1 , 2, 1, 3, 2  } ;
                
                mDefaultIBO = GenerateIBO(indices, 6);
            }
            eglGetError();
            return mDefaultIBO;
            
        }
        unsigned
        GL32Render::GenerateDefaultVBO()
        {
            if ( !mDefaultVBO )
            {
                Vertex v [4] ;
                unsigned short minS = 0;
                unsigned short maxS = 0xFFFF;
                unsigned short minT = 0;
                unsigned short maxT = 0xFFFF;
                v [0] = Vertex( glm::vec4(0.f,0.f,0.f,1.f), minS,maxT );
                v [1] = Vertex( glm::vec4(1.f,0.f,0.f,1.f), maxS,maxT );
                v [2] = Vertex( glm::vec4(0.f,1.f,0.f,1.f), minS,minT );                
                v [3] = Vertex( glm::vec4(1.f,1.f,0.f,1.f), maxS,minT );
                
                
                mDefaultVBO = GenerateVBO(&v, sizeof(v), sizeof(v) * 4);
            }
            eglGetError();
            return mDefaultVBO;
        }
        
        void
        GL32Render::ClearViewport(float r, float g, float b, float a)
        {
            
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) ;
            eglGetError();
        }
        unsigned int
        GL32Render::GenerateVBO(void* data, size_t stride, size_t length)
        {
            GLuint vbo;
            eglGetError();
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);
            eglGetError();
            std::vector<helios::VAOObj> vaoobj;
             
            vaoobj.push_back ( helios::VAOObj({ e::kVertexAttribPositionPosition, helios::VAOObj::R_FLOAT, 4, sizeof(helios::Vertex), 0, 0 }));
            vaoobj.push_back ( helios::VAOObj({ e::kVertexAttribPositionNormal, helios::VAOObj::R_UBYTE, 3, sizeof(helios::Vertex), offsetof(helios::Vertex,n[0]), 1 }));
            vaoobj.push_back ( helios::VAOObj({ e::kVertexAttribPositionTexCoord, helios::VAOObj::R_USHORT, 2, sizeof(helios::Vertex), 16, 1}));
            vaoobj.push_back ( helios::VAOObj({ e::kVertexAttribPositionBoneId, helios::VAOObj::R_SHORT, 1, sizeof(helios::Vertex), 20, 0 }));
            vaoobj.push_back ( helios::VAOObj({ e::kVertexAttribPositionDiffuseColor, helios::VAOObj::R_UBYTE, 4, sizeof(helios::Vertex), 24, 1}));
            
            mVAOMap[vbo] = GenerateVAO(vaoobj);
            
            return vbo;
        }
        unsigned int
        GL32Render::GenerateVAO(const std::vector<VAOObj> & data )
        {
            GLuint vao;
            eglGetError();
            glGenVertexArrays(1, &vao);
            eglGetError();
            glBindVertexArray(vao);
            eglGetError();
            auto it = data.begin();
            auto ite = data.end();
            for ( ; it != ite ; ++ it )
            {
                if(it->name >= 0) {
                    glEnableVertexAttribArray((*it).name);
                    eglGetError();
                    glVertexAttribPointer((*it).name, (GLsizei)(*it).size, (*it).datatype, ((*it).normalize ? GL_TRUE : GL_FALSE), (GLsizei)(*it).stride, BUFFER_OFFSET((*it).offset));
                    eglGetError();
                }
            }
            eglGetError();
            
            glBindVertexArray(0);
            eglGetError();
            return vao;
        }
        unsigned int
        GL32Render::GenerateIBO(unsigned* indices, size_t size)
        {
            assert("This platform does not support 32-bit indices.");
            return 0;
        }
        unsigned int
        GL32Render::GenerateIBO(unsigned short* indices, size_t size)
        {
            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(short), &indices[0], GL_STATIC_DRAW);
            eglGetError();
            return ibo;
        }
        void
        GL32Render::BindVBO ( unsigned int vbo )
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            BindVAO(mVAOMap[vbo]);
            eglGetError();
        }
        void
        GL32Render::BindVAO ( unsigned int vao )
        {
            glBindVertexArray ( vao ) ;
            eglGetError();
        }
        void
        GL32Render::DeleteVBO ( unsigned int vbo )
        {
            glDeleteBuffers(1, &vbo);
            DeleteVAO(mVAOMap[vbo]);
            eglGetError();
        }
        void
        GL32Render::DeleteVAO ( unsigned int vao )
        {
            glDeleteVertexArrays(1, &vao);
            eglGetError();
        }
        void
        GL32Render::PushRenderCommand ( std::vector<RenderCommand> const & r)
        {
            for ( auto it = r.begin() ; it != r.end() ; ++it ) {
                
                if((*it).iboSize == 0)
                    continue;
                unsigned hash = RenderGroup::Hash((*it));
                mGroups[hash].commands.push_back((*it));
                mGroups[hash].vbo = (*it).vbo;
                mGroups[hash].ibo = (*it).ibo;
                mGroups[hash].tex = (*it).tex;
                mGroups[hash].hash = hash;
            }
            eglGetError();
        }
        void
        GL32Render::SetUniforms(std::vector<UniformData_ptr>& uniformdata, Shader& currentShader, Shader& targetShader, int clear)
        {
            for ( auto jt = uniformdata.begin() ; jt != uniformdata.end() ; ++jt )
            {
                UniformData ud = (*(*jt));
                const std::string * u_name = ud.name;
                int name;
                
                if( currentShader.name != targetShader.name )
                {
                    name = currentShader.GetRemappedUniform(targetShader, u_name);
                    if(name < 0) continue;
                }
                else
                {
                    name = currentShader.GetUniform(*u_name);
                    if(name < 0) continue;
                }
                
                switch(ud.type)
                {
                    case UniformData::kUTFloat:
                    {
                        glUniform1fv(name, (GLsizei)ud.count, ud.GetData<GLfloat>());
                        eglGetError();
                    }
                        break;
                    case UniformData::kUTInt:
                    {
                        glUniform1iv(name,(GLsizei)ud.count, ud.GetData<GLint>());
                        eglGetError();
                    }
                        break;
                    case UniformData::kUTVec3:
                    {
                        glUniform3fv(name, (GLsizei)ud.count, ud.GetData<GLfloat>());
                        eglGetError();
                    }
                        break;
                    case UniformData::kUTVec4:
                    {
                        glUniform4fv(name, (GLsizei)ud.count, ud.GetData<GLfloat>());
                        eglGetError();
                    }
                        break;
                    case UniformData::kUTMat3:
                    {
                        glUniformMatrix3fv(name,(GLsizei)ud.count,GL_FALSE,ud.GetData<GLfloat>());
                        eglGetError();
                    }
                        break;
                    case UniformData::kUTMat4:
                    {
                        glUniformMatrix4fv(name,(GLsizei)ud.count,GL_FALSE, ud.GetData<GLfloat>());
                        eglGetError();
                    }
                        break;
                }
            }
            eglGetError();
        }
        void
        GL32Render::RenderShadows(std::vector<RenderGroup>& gVec)
        {
            
            glDisable(GL_BLEND);
            
            glClear(GL_STENCIL_BUFFER_BIT);
            
            glStencilFunc(GL_ALWAYS, 0, ~0);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GREATER);
            
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glDepthMask(GL_FALSE);
            glUseProgram(mShader[e::kRenderStageStencilShadows]);
            mCurrentShader = mShader[e::kRenderStageStencilShadows];
            
            glDisable(GL_CULL_FACE);
            glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
            glStencilOpSeparate(GL_BACK , GL_KEEP, GL_INCR_WRAP, GL_KEEP);
            
            RenderStage(e::kRenderStageStencilShadows, gVec);
            
            glEnable(GL_CULL_FACE);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            
            
            glDepthFunc(GL_LEQUAL);
            
            glStencilFunc(GL_NOTEQUAL, 0, ~0);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDepthMask(GL_TRUE);
                  
            
            RenderStage(e::kRenderStageShadowFill,gVec);
            
            
            glDepthFunc(GL_LESS);
            glStencilFunc(GL_ALWAYS, 0, ~0);
            eglGetError();
        }
        void
        GL32Render::RenderStage(e::RenderStage stage,  std::vector<RenderGroup>& gVec)
        {
            
            Shader currentShader;
            Shader targetShader ;
            GLuint tex = 0;//mCurrentTex;
            GLuint vbo = 0;//mCurrentVBO;
            GLuint ibo = 0;//mCurrentIBO;

            
            auto it = gVec.begin();
            auto ite = gVec.end();
            
            currentShader = mShaderManager.GetShader(mShader[stage]);
            targetShader = mShaderManager.GetShader(mShader[stage]);
            
            glUseProgram(currentShader.name);
            
            
            eglGetError();
            
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
                    BindVBO(vbo);
                    
                }
                if(ibo != (*it).ibo)
                {
                    ibo = (*it).ibo;
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                    eglGetError();
                }
                
                // TODO: Replace with light component.
                {
                    int lp = currentShader.GetUniform(e::kVertexUniformLightPos);
                    
                    
                    if(lp > -1)
                    {
                        glUniform4f(lp, 10.f,25.f,15.f,1.f);
                    }
                    
                    
                    eglGetError();
                }
                for ( auto jt = (*it).commands.begin() ; jt != (*it).commands.end() ; ++jt) {
                    
                    // If this object doesn't participate in this render stage, continue.
                    if((stage & (*jt).state.mask)) {
                        
                        SetUniforms((*jt).uniforms,currentShader,targetShader);
                        eglGetError();
                        
                        glDrawElements(GL_TRIANGLES, (*jt).iboSize , GL_UNSIGNED_SHORT, (void*)((*jt).iboOffset*sizeof(short)));
                        
                        eglGetError();
                    }
                }
            }
            eglGetError();
            
        }
        
        void
        GL32Render::Render()
        {
            
            std::vector<RenderGroup> gVec;
            std::transform(mGroups.begin(), mGroups.end(), std::back_inserter(gVec), boost::bind(&std::map<unsigned,RenderGroup>::value_type::second,_1));
            
            if ( mOptions & RenderOptions_Blend ) {
                std::sort<helios::RenderGroup>(gVec.begin(),gVec.end());
            } else {
                glDisable(GL_BLEND);
                glDisable(GL_CULL_FACE);
            }
            
            RenderStage(e::kRenderStageGeometry,gVec);
            
            if(mOptions & RenderOptions_StenciledShadowVolumes)
                RenderShadows(gVec);
            
            ClearRenderStack();
            eglGetError();
            
        }
        void
        GL32Render::ClearRenderStack()
        {
            mGroups.clear();
            eglGetError();
        }
        void
        GL32Render::SetViewport(int x, int y, int w, int h)
        {
            if( mCurrentViewport.x != x || mCurrentViewport.y != y || mCurrentViewport.w != w || mCurrentViewport.h != h){
                glViewport(x,y,w,h);
                glScissor(x,y,w,h);
                mCurrentViewport.x = x;
                mCurrentViewport.y = y;
                mCurrentViewport.w = w;
                mCurrentViewport.h = h;
            }
            eglGetError();
        }
        unsigned
        GL32Render::LoadShader(std::string & vertex_file, std::string & fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms, std::string defines)
        {
            std::string s="";
            return LoadShader(vertex_file,s,fragment_file,attribs,uniforms);
        }
        unsigned
        GL32Render::LoadShader(std::string &vertex_file, std::string &geometry_file, std::string &fragment_file, std::map<std::string, int>& attribs, std::map<std::string, int>& uniforms, std::string defines)
        {
            eglGetError();
            std::cout << "Loading " << vertex_file << std::endl;
            if(mShaderManager.ShaderLoaded(vertex_file + fragment_file, uniforms))
                return mShaderManager.GetShader(vertex_file + fragment_file).name;
            
            std::string vs = IRender::LoadFileText(vertex_file);
            std::string fs = IRender::LoadFileText(fragment_file);
            
            // D_PRINT("%s: %s", vertex_file.c_str(), vs.c_str());
            GLuint vshad = glCreateShader(GL_VERTEX_SHADER);
            GLuint fshad = glCreateShader(GL_FRAGMENT_SHADER);
            GLuint prog = glCreateProgram();
            
            const char * cstr = vs.c_str();
            eglGetError();
            glShaderSource(vshad, 1, &cstr, NULL);
            glCompileShader(vshad);
            getShaderLog(vshad);
            glAttachShader(prog, vshad);
            cstr = fs.c_str();
            eglGetError();
            glShaderSource(fshad, 1, &cstr, NULL);
            
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
            glBindFragDataLocation(prog,0,e::kColorOut.c_str());
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
            s.filename = vertex_file + fragment_file;
            s.uniforms = uniforms;
            s.attribs = attribs;
            
            mShaderManager.AddShader(s);
            
            eglGetError();
            return prog;
        }
        void
        GL32Render::DeleteShader(unsigned int shader)
        {
            mShaderManager.RemoveShader(mShaderManager.GetShader(shader));
            glDeleteShader(shader);
            eglGetError();
        }
        
        void
        GL32Render::GenerateFBO(e::RenderStage r, size_t w, size_t h, int colorAttachments)
        {
            GLuint fbo;
            
            colorAttachments = (colorAttachments > MAX_COLOR_ATTACHMENTS ? MAX_COLOR_ATTACHMENTS : colorAttachments);
            
            GLuint tex[2+colorAttachments];
            GLuint rbo;
            FBO f;
            int i ;
            
            glGenRenderbuffers(1, &rbo);
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glGenTextures(2+colorAttachments, tex);
            
            for ( i = 0 ; i < colorAttachments ; ++i ) {
                
                f.colorBufferTex[i] = tex[i];
                glBindTexture(GL_TEXTURE_2D, tex[i]);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mCurrentViewport.w, mCurrentViewport.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, tex[i], 0);
            }
            
            glBindTexture(GL_TEXTURE_2D, tex[i]);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mCurrentViewport.w, mCurrentViewport.h, 0, GL_DEPTH_COMPONENT,GL_UNSIGNED_INT, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex[i], 0);
            
            glBindTexture(GL_TEXTURE_2D, tex[i+1]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mCurrentViewport.w, mCurrentViewport.h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex[i+1], 0);
            
            
            f.name = fbo;
            f.depthBufferTex = tex[i];
            f.stencilBufferTex = tex[i+1];
            
            mFBO[r] = f;
            glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFBO);
            eglGetError();
        }
        void
        GL32Render::SetUniform(int shader_name, UniformData_ptr ud)
        {
            for(auto it = mUniforms.begin() ; it != mUniforms.end() ; ++it)
            {
                if((*it).uniform->name == ud->name) {
                    (*it).uniform = ud;
                    return;
                }
            }
            UniformPair up ;
            up.shader_id = shader_name;
            up.uniform = ud;
            mUniforms.push_back(up);
            eglGetError();
        }
        void
        GL32Render::SetRenderStage(e::RenderStage stage, std::vector<UniformData> parameters)
        {
            if(mFBO.find(stage) != mFBO.end())
            {
                glDeleteFramebuffers(1, &mFBO[stage].name);
            }
            GenerateFBO(stage,mCurrentViewport.w,mCurrentViewport.h);
            eglGetError();
        }
    };
    
};
