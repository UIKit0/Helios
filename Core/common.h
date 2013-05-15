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
        extern std::string const kComponentBase       ;
        extern std::string const kComponentRenderable ;
        extern std::string const kComponentPosition   ;
        extern std::string const kComponentScriptable ;
        extern std::string const kComponentTouchable  ;
        extern std::string const kComponentPhysics    ;
        extern std::string const kComponentSkeleton   ;
        extern std::string const kComponentNetwork    ;
        extern std::string const kComponentCamera     ;
        extern std::string const kComponentRespondsToKeyboard;
        extern std::string const kComponentRespondsToMouse;
        extern std::string const kComponentTextureAnimation;
        extern std::string const kComponentLight      ;
        
        extern std::string const kLayerBase           ;        
        
        extern std::string const kEventTargetTouch    ;
        extern std::string const kEventTargetTouchHit ;
        extern std::string const kEventTargetTime     ;
        extern std::string const kEventTargetBase     ;
        extern std::string const kEventTargetRender   ;
        extern std::string const kEventTargetMatrix   ;
        extern std::string const kEventTargetPosition ;
        extern std::string const kEventTargetUniformData ;
        extern std::string const kEventTargetKeyboard  ;
        extern std::string const kEventTargetMouse     ;
        extern std::string const kEventTargetAnimation ;
        extern std::string const kEventTargetTexture   ;
        extern std::string const kEventTargetAction    ;
        
        extern std::string const kEventTextureMatrix  ;
        extern std::string const kEventTextureMix     ;
        extern std::string const kEventAnimationStart ;
        extern std::string const kEventAnimationEnd   ;
        
        extern std::string const kEventKeyDown        ;
        extern std::string const kEventKeyUp          ;
        extern std::string const kEventKeyModifierChanged ;
        
        extern std::string const kEventMousePos       ;
        extern std::string const kEventMouseButtonDown;
        extern std::string const kEventMouseButtonUp  ;
        
        extern std::string const kEventTouchBegin     ;
        extern std::string const kEventTouchEnd       ;
        extern std::string const kEventTouchMoved     ;
        extern std::string const kEventTouchCancelled ;
        
        extern std::string const kEventBase           ;
        extern std::string const kEventRenderCommand  ;
        extern std::string const kEventMatrix         ;
        extern std::string const kEventUniformData    ;
        
        extern std::string const kEventMatrixModelView;
        extern std::string const kEventMatrixProjection;
        extern std::string const kEventMatrixCamera ;
        
        extern std::string const kEventPositionMatrix;
        extern std::string const kEventTime           ;
        
        extern std::string const kSceneBase           ;

        extern std::string const kActionIdle          ;
        extern std::string const kActionNotFound      ;
        extern std::string const kActionMoveForward   ;
        extern std::string const kActionMoveRight     ;
        extern std::string const kActionMoveLeft      ;
        extern std::string const kActionMoveBack      ;
        extern std::string const kActionRotate        ;   
        
        
        extern std::string const kVertexUniformModelView  ;
        extern std::string const kVertexUniformProjection ;
        extern std::string const kVertexUniformNormalMat  ;
        extern std::string const kVertexUniformTexMat     ;
        extern std::string const kVertexUniformJoints     ;
        extern std::string const kVertexUniformLightPos   ;
        
        extern std::string const kFragmentUniformSampler0 ;
        extern std::string const kFragmentUniformSampler1 ;
        extern std::string const kFragmentUniformSampler2 ;
        extern std::string const kFragmentUniformSampler3 ;
        extern std::string const kFragmentUniformSampler4 ;
        extern std::string const kFragmentUniformSampler5 ;
        extern std::string const kFragmentUniformSampler6 ;
        extern std::string const kFragmentUniformSampler7 ;
        extern std::string const kFragmentUniformShadowmap0;
        
        extern std::string const kFragmentUniformTimeSinceStart ;
        
        extern std::string const kVertexAttribPosition    ;
        extern std::string const kVertexAttribNormal      ;
        extern std::string const kVertexAttribDiffuseColor;
        extern std::string const kVertexAttribTexCoord    ;
        extern std::string const kVertexAttribBoneId      ;
        extern std::string const kVertexAttribExtrudes    ;
        
        // For use with OpenGL 3.2+
        extern std::string const kColorOut             ;
        extern std::string const kNormalsOut           ;
        extern std::string const kPositionOut          ;
        
        enum AttribPositions
        {
            kVertexAttribPositionPosition = 0,
            kVertexAttribPositionNormal,
            kVertexAttribPositionTexCoord,
            kVertexAttribPositionBoneId,
            kVertexAttribPositionExtrudes,
            kVertexAttribPositionDiffuseColor,
        };
        
        enum RenderStage
        {
            kRenderStageGeometry = 1,
            kRenderStageDiffuse =  1<<1,
            kRenderStageStencilShadows = 1<<2,
            kRenderStageShadowFill = 1<<3,
            kRenderStagePostprocess = 1<<4,
            kRenderStageHUD = 1<<5,
            
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
            int id ;
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
        
        struct InputRemap
        {

            InputRemap(float _data[16], std::string const & _action, int _keyCode) : action(_action), keyCode(_keyCode) 
            {
                memcpy(dataf, _data, 16*sizeof(float));
            };
            InputRemap(int _data[16], std::string const & _action, int _keyCode) :  action(_action), keyCode(_keyCode) 
            {
                memcpy(datai, _data, 16*sizeof(int));
            };
            
            union
            {
                float dataf[16];
                int datai[16];  
            };
            std::string const & action;
            int keyCode;
        };

    };
};

#endif
