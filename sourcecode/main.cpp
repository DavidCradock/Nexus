#include "precompiled_header.h"
#include "Nexus.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpStr, INT iShowCmd)
{
    try
        {
            // Attempt to create window
            Nexus::RenderDevice* pRD = Nexus::RenderDevice::getPointer();
            pRD->createWindow("Nexus");
            
            Nexus::Managers* pManagers = Nexus::Managers::getPointer();

            pManagers->gui->loadAllThemes();
            pManagers->textFonts->loadAll();

            // Initialise all applications
            std::string applicationName = "Development";
            Nexus::ApplicationDevelopment applicationDevelopment(applicationName);
            pManagers->applications->callAllApps_initOnce();

            // Load all textures
            pManagers->textures->loadGroup("default");
            pManagers->textures->loadGroup("fonts");
            pManagers->textFonts->loadAll();

            // Main loop
            while (pRD->updateWindow())
            {
                pManagers->input->update(pRD->getWindowFullscreen(), pRD->getWindowWidth(), pRD->getWindowHeight());
                if (!pManagers->applications->callCurrentApp_onUpdate())
                    break;

                pManagers->sprites->update();
                pManagers->sprites->render();
                pManagers->gui->update();
                pManagers->gui->render();
 
                pRD->swapBuffers();
                Sleep(0);
            }
            pManagers->input->shutdown();
            pRD->closeWindow();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::string error = e.what();
        Nexus::Log::getPointer()->add(error, true);
        MessageBox(NULL, error.c_str(), "Exception...", MB_OK);
        __debugbreak();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
