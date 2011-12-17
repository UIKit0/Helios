
#ifndef Helios_ShaderManager_h
#define Helios_ShaderManager_h

#include <map>
#include <string>
#include <Render/RenderCommand.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

namespace helios
{
    struct Shader
    {
        
        std::map<std::string, int> attribs;
        std::map<std::string, int> uniforms;
        unsigned name;
        unsigned count = 0;
        
        std::string filename;
        
        int GetUniform(std::string name)
        {
           if(uniforms.find(name) != uniforms.end())
               return uniforms[name];
            return -1;
        };
        std::string GetUniformName(unsigned target)
        {
            std::string _name;
            
            for(auto it = uniforms.begin() ; it != uniforms.end() ; ++it)
            {
                if(it->second == target)
                    _name = it->first;
            }
            
            return _name;
        };
        int GetRemappedUniform(Shader s,unsigned target)
        {
            std::string _tn = s.GetUniformName(target);
            unsigned i = -1 ;
            for ( auto it = uniforms.begin() ; it != uniforms.end() ; ++it)
            {
                if( _tn == it->first )
                {
                    i = it->second;
                }
            }
            return i ;
        };
    };
    
    class ShaderManager
    {
    private:
        
        std::map<std::string, Shader> mShaders;
        
        std::map<unsigned, Shader*>   mShadersByName;
        
    public:
        
        // Return TRUE if the shader is to be deleted.
        bool RemoveShader(Shader& s)
        {
            s.count--;
            
            if(s.count == 0) {
                mShadersByName.erase(mShadersByName.find(s.name));
                mShaders.erase(mShaders.find(s.filename));
                return TRUE;
            }
            return FALSE;
        };
        void RetainShader(Shader& s)
        {
            s.count++;
        }
        void AddShader(Shader s)
        {
            mShaders[s.filename] = s;
            mShadersByName[s.name] = &mShaders[s.filename];
            mShaders[s.filename].count++;
        };
        bool ShaderLoaded(std::string _name)
        {
            return mShaders.find(_name) != mShaders.end();
        };
        bool ShaderLoaded(std::string _name, std::map<std::string, int> & _uniforms)
        {
            auto it = mShaders.find(_name);
            
            if((it != mShaders.end())) {
                _uniforms = (*it).second.uniforms; 
                return true;
            }
            
            return false;
        };
        bool ShaderLoaded(unsigned name, std::map<std::string, int> & _uniforms)
        {
            auto it = mShadersByName.find(name) ;
            if( it != mShadersByName.end() )
            {
                _uniforms = (*it).second->uniforms;
                return true;
            }
            return false;
        }
        bool ShaderLoaded(unsigned name)
        {
            return (mShadersByName.find(name) != mShadersByName.end());
        };
        Shader & GetShader(unsigned name)
        {   
            
            return *(mShadersByName[name]);
            
          ;
        };
        Shader & GetShader(std::string name)
        {
            
            
            return mShaders[name];
            
            
        };
    };
}

#endif
