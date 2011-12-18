//
//  common.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_common_h
#define Helios_common_h
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

#include <limits.h>

#define USE_QUATERNIONS 0

#ifdef DEBUG
#define FLE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
/* just a helper for code location */
#define LOC std::cout << std::endl << std::string(FLE) << ":" << __LINE__ << ":: ";

/* macro using var args */
#define D_PRINT(...) LOC printf(__VA_ARGS__);

/* macro for general debug print statements. */
#define D_VAL(text) LOC std::cout << text << std::endl;

/* macro that dumps a variable name and its actual value */
#define D_VAR(text) LOC std::cout << (#text) << " : " << text << std::endl;

#else

/* when debug isn't defined all the macro calls do absolutely nothing */
#define D_PRINT(fmt,...)
#define D_VAL(text)
#define D_VAR(text)

#endif /* DEBUG */
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

//#include <map>
namespace helios
{
    namespace e
    {
        std::string const kComponentBase       = "com.helios.component.Base";
        std::string const kComponentRenderable = "com.helios.component.Renderable";
        std::string const kComponentPosition   = "com.helios.component.Position";
        std::string const kComponentScriptable = "com.helios.component.Scriptable";
        std::string const kComponentTouchable  = "com.helios.component.Touchable";
        std::string const kComponentPhysics    = "com.helios.component.Physics";
        std::string const kComponentSkeleton   = "com.helios.component.Skeleton";
        std::string const kComponentNetwork    = "com.helios.component.Network";
        std::string const kComponentCamera     = "com.helios.component.Camera";
        std::string const kComponentRespondsToKeyboard = "com.helios.component.Keyboard";
        std::string const kComponentTextureAnimation = "com.helios.component.TextureAnimation";
        
        std::string const kLayerBase           = "com.helios.layer.Base";        
        
        std::string const kEventTargetTouch    = "com.helios.eventtarget.Touch";
        std::string const kEventTargetTouchHit = "com.helios.eventtarget.TouchHit";
        std::string const kEventTargetTime     = "com.helios.eventtarget.Time";
        std::string const kEventTargetBase     = "com.helios.eventtarget.Base";
        std::string const kEventTargetRender   = "com.helios.eventtarget.Render";
        std::string const kEventTargetMatrix   = "com.helios.eventtarget.Matrix";
        std::string const kEventTargetPosition = "com.helios.eventtarget.Position";
        std::string const kEventTargetUniformData = "com.helios.eventtarget.UniformData";
        std::string const kEventTargetKeyboard = "com.helios.eventtarget.Keyboard";
        std::string const kEventTargetAnimation= "com.helios.eventtarget.Animation";
        std::string const kEventTargetTexture  = "com.helios.eventtarget.Texture";
        
        std::string const kEventTextureMatrix  = "com.helios.event.TextureMatrix";
        std::string const kEventTextureMix     = "com.helios.event.TextureMix";
        std::string const kEventAnimationStart = "com.helios.event.AnimationStart";
        std::string const kEventAnimationEnd   = "com.helios.event.AnimationEnd";
        
        std::string const kEventKeyDown        = "com.helios.event.KeyDown";
        std::string const kEventKeyUp          = "com.helios.event.KeyUp";
        std::string const kEventKeyModifierChanged = "com.helios.event.KeyModifierChanged";
        
        std::string const kEventTouchBegin     = "com.helios.event.TouchBegin";
        std::string const kEventTouchEnd       = "com.helios.event.TouchEnd";
        std::string const kEventTouchMoved     = "com.helios.event.TouchMoved";
        std::string const kEventTouchCancelled = "com.helios.event.TouchCancelled";
        
        std::string const kEventBase           = "com.helios.event.Base";
        std::string const kEventRenderCommand  = "com.helios.event.RenderCommand";
        std::string const kEventMatrix         = "com.helios.event.Matrix";
        std::string const kEventUniformData       = "com.helios.event.UniformData";
        
        std::string const kEventMatrixModelView     = "com.helios.event.MatrixModelView";
        std::string const kEventMatrixProjection    = "com.helios.event.MatrixProjection";
        std::string const kEventMatrixCamera        = "com.helios.event.MatrixCamera";
        
        std::string const kEventPositionMatrix = "com.helios.event.MatrixPosition";
        std::string const kEventTime           = "com.helios.event.Time";
        
        std::string const kSceneBase           = "com.helios.scene.Base";
        
        
        std::string const kVertexUniformModelView  = "UNIFORM_MV";
        std::string const kVertexUniformProjection = "UNIFORM_P";
        std::string const kVertexUniformNormalMat  = "UNIFORM_NM";
        std::string const kVertexUniformTexMat     = "UNIFORM_TEXMAT";
        std::string const kVertexUniformJoints     = "UNIFORM_JOINTS";
        std::string const kVertexUniformLightPos   = "UNIFORM_LIGHTPOS";
        
        std::string const kFragmentUniformSampler0 = "UNIFORM_SAMPLER0";
        std::string const kFragmentUniformSampler1 = "UNIFORM_SAMPLER1";
        std::string const kFragmentUniformSampler2 = "UNIFORM_SAMPLER2";
        std::string const kFragmentUniformSampler3 = "UNIFORM_SAMPLER3";
        std::string const kFragmentUniformSampler4 = "UNIFORM_SAMPLER4";
        std::string const kFragmentUniformSampler5 = "UNIFORM_SAMPLER5";
        std::string const kFragmentUniformSampler6 = "UNIFORM_SAMPLER6";
        std::string const kFragmentUniformSampler7 = "UNIFORM_SAMPLER7";
        std::string const kFragmentUniformShadowmap0 = "UNIFORM_SHADOWMAP0";
        
        std::string const kFragmentUniformTimeSinceStart = "UNIFORM_TIME_SINCE_START";
        
        std::string const kVertexAttribPosition    = "ATTR_POS";
        std::string const kVertexAttribNormal      = "ATTR_NORM";
        std::string const kVertexAttribDiffuseColor= "ATTR_DIFFUSE";
        std::string const kVertexAttribTexCoord    = "ATTR_TEX";
        std::string const kVertexAttribBoneId      = "ATTR_BONEID";
        std::string const kVertexAttribNoExtrude   = "ATTR_NO_EXTRUDE";
        std::string const kVertexAttribNoBones     = "ATTR_NO_BONES"; 
        // For use with OpenGL 3.2+
        std::string const kFragmentOut             = "OUT_FRAG";
        
        enum AttribPositions
        {
            kVertexAttribPositionPosition = 0,
            kVertexAttribPositionNormal,
            kVertexAttribPositionDiffuseColor,
            kVertexAttribPositionTexCoord,
            kVertexAttribPositionBoneId,
        //    kVertexAttribPositionNoExtrude,
            kVertexAttribPositionNoBones
        };
        
        enum RenderStage
        {
            kRenderStageGeometry,
            kRenderStageDiffuse,
            kRenderStageShadows,
            kRenderStagePostprocess
        };
        
    };
    namespace s
    {
        struct Frame
        {
            float time;
#if(USE_QUATERNIONS == 1)
            glm::quat rotation;
            glm::vec3 translation;
#else
            glm::mat4 mat;
#endif
        }; 
        struct Joint
        {
            glm::vec3 absTrans;
            glm::vec3 relTrans;
            glm::quat absQuat;
            glm::quat relQuat;
            glm::mat4 absMat;
            glm::mat4 relMat;
            std::vector<Frame> keyframes;
            //size_t keyframes_count = 0;
            uint8_t   jointid;
            int8_t   parent;
        };
        
        struct TexFrame
        {
            float time;
            float minS, minT, maxS, maxT;
            float mix;
            uint8_t color[4];
        };
        struct Animation
        {
            float startTime, endTime, fpsDelta, currentTime;
            bool loop;
            
        };
        struct MaterialGroup
        {
            
            struct
            {
                unsigned short start;
                unsigned short end;
            } iboRange;
            unsigned short ibo;
            unsigned short tex;
            
        };

    };
};

#endif
