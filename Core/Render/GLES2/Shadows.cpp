#include <Render/GLES2/Shadows.h>
#include <iostream>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace helios
{
    namespace gles2
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
