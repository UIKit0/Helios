
#ifndef Helios_GLES2_Shadows_h
#define Helios_GLES2_Shadows_h
#include <Render/IRender.h>
#include <Render/TextureAtlas.hpp>
#include <Render/Texture.hpp>
#include <Render/ShaderManager.h>
#include <Render/GL32/Lights.h>

#include <glm/glm.hpp>

namespace helios
{
    namespace gl32
    {
        
        
        class Shadows
        {
        private:
            ShaderManager& _shaderManager;
           
            Lights&        _lights;
            
            IRender *      _render;
            
        public:
            Shadows(ShaderManager& shaderManager, Lights& lights, RenderOptions options, IRender* render);
            ~Shadows();
            
            void RenderStencilShadowVolumes(std::vector<RenderGroup>& gVec);
            void RenderShadowMaps(std::vector<RenderGroup>& gVec, unsigned targetFBO);
            
        };
    };
};


#endif
