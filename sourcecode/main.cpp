#include "precompiled_header.h"
#include "applications/applicationManager.h"
#include "applications/applicationDevelopment.h"
#include "graphics/renderDevice.h"
#include "graphics/texture.h"
#include "input/inputManager.h"
#include "graphics/shader.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpStr, INT iShowCmd)
{
    try
    {
        // Attempt to create window
        Nexus::RenderDevice* pRD = Nexus::RenderDevice::getPointer();
        pRD->createWindow("Nexus");

        // Initialise input
        Nexus::InputManager* pIM = Nexus::InputManager::getPointer();
        pIM->init(pRD->getWindowHandle());

        // Shaders
        Nexus::ShaderManager* pSM = Nexus::ShaderManager::getPointer();
        pSM->addShader("sprites", "shaders/sprites.vs", "shaders/sprites.fs", "default");
        pSM->addShader("textFont", "shaders/textFont.vs", "shaders/textFont.fs", "default");
        pSM->loadGroup("default");

        // Texture manager groups
        Nexus::TextureManager* pTM = Nexus::TextureManager::getPointer();
        pTM->addNewGroup("fonts");

        // Initialise all applications
        Nexus::ApplicationManager* pAM = Nexus::ApplicationManager::getPointer();
        std::string applicationName = "Development";
        Nexus::ApplicationDevelopment applicationDevelopment(applicationName);
        pAM->callAllApps_initOnce();

        // Main loop
        while (pRD->updateWindow())
        {
            pIM->update(pRD->getWindowFullscreen(), pRD->getWindowWidth(), pRD->getWindowHeight());
            if (!pAM->callCurrentApp_onUpdate())
                break;

            pRD->swapBuffers();
            Sleep(0);
        }
        pIM->shutdown();
        pRD->closeWindow();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::string error = e.what();
        MessageBox(NULL, error.c_str(), "Exception...", MB_OK);

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
