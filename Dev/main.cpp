#include <GL/glfw.h>
#include <Helios.h>
#include <Render/GL32/Render.h>
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <stdint.h>

#include "Scene/BasicScene.h"

#define W 684
#define H 384

int main()
{
    GLint running = GL_TRUE;
    GLint r = glfwInit();
    
    
    assert(r);
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);       // Force OGL 3.2
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetWindowTitle("Helios Engine Development");
    
    glfwSwapInterval(1);    // Enable vsync.
    
    r = glfwOpenWindow(W,H,8,8,8,8,24,8,GLFW_WINDOW);       // Set color to RGBA8888, depth to 24-bit, stencil to 8-bit.
    
    assert(r);
    
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
    
    helios::SceneManager::Inst().SetScreenSize(W,H,72);
    helios::SceneManager::Inst().RegisterScene("BasicScene", new helios_dev::BasicScene() );
    helios::SceneManager::Inst().SwitchScene("BasicScene");
    
    while ( running )
    {
        uint64_t t (glfwGetTime() * 1.0e6);
        helios::SceneManager::Inst().DispatchEvents();
        helios::SceneManager::Inst().FireUpdates(t);
        helios::SceneManager::Inst().Process();
        helios::SceneManager::Inst().Render(t);
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam(GLFW_OPENED);
    }
    
    glfwTerminate();
    
    return(0);
}