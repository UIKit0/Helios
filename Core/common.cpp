#include <string>
namespace helios
{
    namespace e
    {
        extern std::string const kComponentBase       = "com.helios.component.Base";
        extern std::string const kComponentRenderable = "com.helios.component.Renderable";
        extern std::string const kComponentPosition   = "com.helios.component.Position";
        extern std::string const kComponentScriptable = "com.helios.component.Scriptable";
        extern std::string const kComponentTouchable  = "com.helios.component.Touchable";
        extern std::string const kComponentPhysics    = "com.helios.component.Physics";
        extern std::string const kComponentSkeleton   = "com.helios.component.Skeleton";
        extern std::string const kComponentNetwork    = "com.helios.component.Network";
        extern std::string const kComponentCamera     = "com.helios.component.Camera";
        extern std::string const kComponentRespondsToKeyboard = "com.helios.component.Keyboard";
        extern std::string const kComponentRespondsToMouse = "com.helios.component.Mouse";
        extern std::string const kComponentTextureAnimation = "com.helios.component.TextureAnimation";
        
        extern std::string const kLayerBase           = "com.helios.layer.Base";        
        
        extern std::string const kEventTargetTouch    = "com.helios.eventtarget.Touch";
        extern std::string const kEventTargetTouchHit = "com.helios.eventtarget.TouchHit";
        extern std::string const kEventTargetTime     = "com.helios.eventtarget.Time";
        extern std::string const kEventTargetBase     = "com.helios.eventtarget.Base";
        extern std::string const kEventTargetRender   = "com.helios.eventtarget.Render";
        extern std::string const kEventTargetMatrix   = "com.helios.eventtarget.Matrix";
        extern std::string const kEventTargetPosition = "com.helios.eventtarget.Position";
        extern std::string const kEventTargetUniformData = "com.helios.eventtarget.UniformData";
        extern std::string const kEventTargetKeyboard = "com.helios.eventtarget.Keyboard";
        extern std::string const kEventTargetMouse    = "com.helios.eventtarget.Mouse";
        extern std::string const kEventTargetAnimation= "com.helios.eventtarget.Animation";
        extern std::string const kEventTargetTexture  = "com.helios.eventtarget.Texture";
        extern std::string const kEventTargetAction   = "com.helios.eventtarget.Action";
        
        extern std::string const kEventTextureMatrix  = "com.helios.event.TextureMatrix";
        extern std::string const kEventTextureMix     = "com.helios.event.TextureMix";
        extern std::string const kEventAnimationStart = "com.helios.event.AnimationStart";
        extern std::string const kEventAnimationEnd   = "com.helios.event.AnimationEnd";
        
        extern std::string const kEventKeyDown        = "com.helios.event.KeyDown";
        extern std::string const kEventKeyUp          = "com.helios.event.KeyUp";
        extern std::string const kEventKeyModifierChanged = "com.helios.event.KeyModifierChanged";
        
        extern std::string const kEventMousePos       = "com.helios.event.MousePos";
        extern std::string const kEventMouseButtonDown= "com.helios.event.MouseButtonDown";
        extern std::string const kEventMouseButtonUp  = "com.helios.event.MouseButtonUp";        
        
        extern std::string const kEventTouchBegin     = "com.helios.event.TouchBegin";
        extern std::string const kEventTouchEnd       = "com.helios.event.TouchEnd";
        extern std::string const kEventTouchMoved     = "com.helios.event.TouchMoved";
        extern std::string const kEventTouchCancelled = "com.helios.event.TouchCancelled";
        
        extern std::string const kEventBase           = "com.helios.event.Base";
        extern std::string const kEventRenderCommand  = "com.helios.event.RenderCommand";
        extern std::string const kEventMatrix         = "com.helios.event.Matrix";
        extern std::string const kEventUniformData    = "com.helios.event.UniformData";
        
        extern std::string const kEventMatrixModelView     = "com.helios.event.MatrixModelView";
        extern std::string const kEventMatrixProjection    = "com.helios.event.MatrixProjection";
        extern std::string const kEventMatrixCamera        = "com.helios.event.MatrixCamera";
        
        extern std::string const kEventPositionMatrix = "com.helios.event.MatrixPosition";
        extern std::string const kEventTime           = "com.helios.event.Time";
        
        extern std::string const kSceneBase           = "com.helios.scene.Base";

        extern std::string const kActionIdle          = "com.helios.action.Idle";
        extern std::string const kActionNotFound      = "com.helios.action.NotFound";
        extern std::string const kActionMoveForward   = "com.helios.action.MoveForward";   
        extern std::string const kActionMoveRight     = "com.helios.action.MoveRight";
        extern std::string const kActionMoveLeft      = "com.helios.action.MoveLeft";
        extern std::string const kActionMoveBack      = "com.helios.action.MoveBack";

        extern std::string const kActionRotate        = "com.helios.action.Rotate"; // i.e. quaternion describing rotation
        
        
        extern std::string const kVertexUniformModelView  = "UNIFORM_MV";
        extern std::string const kVertexUniformProjection = "UNIFORM_P";
        extern std::string const kVertexUniformNormalMat  = "UNIFORM_NM";
        extern std::string const kVertexUniformTexMat     = "UNIFORM_TEXMAT";
        extern std::string const kVertexUniformJoints     = "UNIFORM_JOINTS";
        extern std::string const kVertexUniformLightPos   = "UNIFORM_LIGHTPOS";
        
        extern std::string const kFragmentUniformSampler0 = "UNIFORM_SAMPLER0";
        extern std::string const kFragmentUniformSampler1 = "UNIFORM_SAMPLER1";
        extern std::string const kFragmentUniformSampler2 = "UNIFORM_SAMPLER2";
        extern std::string const kFragmentUniformSampler3 = "UNIFORM_SAMPLER3";
        extern std::string const kFragmentUniformSampler4 = "UNIFORM_SAMPLER4";
        extern std::string const kFragmentUniformSampler5 = "UNIFORM_SAMPLER5";
        extern std::string const kFragmentUniformSampler6 = "UNIFORM_SAMPLER6";
        extern std::string const kFragmentUniformSampler7 = "UNIFORM_SAMPLER7";
        extern std::string const kFragmentUniformShadowmap0 = "UNIFORM_SHADOWMAP0";
        
        extern std::string const kFragmentUniformTimeSinceStart = "UNIFORM_TIME_SINCE_START";
        
        extern std::string const kVertexAttribPosition    = "ATTR_POS";
        extern std::string const kVertexAttribNormal      = "ATTR_NORM";
        extern std::string const kVertexAttribDiffuseColor= "ATTR_DIFFUSE";
        extern std::string const kVertexAttribTexCoord    = "ATTR_TEX";
        extern std::string const kVertexAttribBoneId      = "ATTR_BONEID";
        extern std::string const kVertexAttribNoExtrude   = "ATTR_NO_EXTRUDE";
        extern std::string const kVertexAttribNoBones     = "ATTR_NO_BONES"; 
        // For use with OpenGL 3.2+
        extern std::string const kColorOut                = "OUT_COLOR";
        extern std::string const kNormalsOut              = "OUT_NORMALS";
        extern std::string const kPositionOut             = "OUT_POSITION";
    }
}