#include <GL/glfw.h>
#include <Helios.h>
#include <Render/GL32/Render.h>
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <stdint.h>

#include "Scene/BasicScene.h"

int main()
{
    GLint running = GL_TRUE;
    GLint r = glfwInit();
    
    
    assert(r);
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetWindowTitle("Helios Engine Development");
    
    glfwSwapInterval(1);
    
    r = glfwOpenWindow(1368,768,8,8,8,8,24,8,GLFW_WINDOW);
    
    assert(r);
    
    D_PRINT("%s\n", glGetString(GL_VERSION));
    
    helios::GL32Render * render = new helios::GL32Render();
    helios::RenderOptions opt = helios::RenderOptions_StenciledShadowVolumes;
    render->SetOptions(opt);
    
    {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
        CFStringRef str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
        CFRelease(resourcesURL);
        char path[PATH_MAX];

        CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
        CFRelease(str);
        
        helios::SceneManager::Inst().Setup(render,std::string(path) + "/Contents/Resources");
        
    }
    
    helios::SceneManager::Inst().RegisterScene("BasicScene", new helios_dev::BasicScene() );
    helios::SceneManager::Inst().SwitchScene("BasicScene");
    
    while ( running )
    {
        uint64_t t (glfwGetTime() * 1.0e6);
        helios::SceneManager::Inst().DispatchEvents();
        helios::SceneManager::Inst().FireUpdates(t);
        helios::SceneManager::Inst().Process();
        t = uint64_t(glfwGetTime() * 1.0e6);
        helios::SceneManager::Inst().Render(t);
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam(GLFW_OPENED);
    }
    
    glfwTerminate();
    
    return(0);
}