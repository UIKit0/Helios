#include <Render/GL32/Shadows.h>
#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

namespace helios
{
    namespace gl32
    {
        Shadows::Shadows(ShaderManager& shaderManager, Lights& lights, RenderOptions options, IRender* render)
        : _shaderManager(shaderManager), _lights(lights), _render(render)
        {
            
        }
        void 
        Shadows::RenderStencilShadowVolumes(std::vector<RenderGroup>& gVec)
        {
            
        }
        void 
        Shadows::RenderShadowMaps(std::vector<RenderGroup>& gVec, unsigned targetFBO)
        {
            
        }
    }
}
