#include "precompiled_header.h"
#include "Nexus.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpStr, INT iShowCmd)
{
    try
        {
            // Attempt to create window
            Nexus::RenderDevice* pRD = Nexus::RenderDevice::getPointer();
            pRD->createWindow("Nexus");

            // Initialise input
            Nexus::ManagerInputDevices* pManInputDevices = Nexus::ManagerInputDevices::getPointer();
            pManInputDevices->init(pRD->getWindowHandle());

            // Timing
            Nexus::Timing timing;
            timing.setStatFPSSrate(1);

            // Shaders
            Nexus::ManagerShaders* pManShaders = Nexus::ManagerShaders::getPointer();
            pManShaders->addNewGroup("default");
            pManShaders->addShader("sprites", "shaders/simple.vs", "shaders/simple.fs", "default");
            pManShaders->addShader("sprites", "shaders/sprites.vs", "shaders/sprites.fs", "default");
            pManShaders->addShader("textFont", "shaders/textFont.vs", "shaders/textFont.fs", "default");
            pManShaders->addShader("gui", "shaders/gui.vs", "shaders/gui.fs", "default");
            pManShaders->loadGroup("default");

            // Texture manager groups
            Nexus::ManagerTextures* pManTextures = Nexus::ManagerTextures::getPointer();
            pManTextures->addNewGroup("default");

            // GUI
            Nexus::ManagerGUI* pManGUI = Nexus::ManagerGUI::getPointer();
            // Create default theme and set as default
            Nexus::GUITheme* pTheme = pManGUI->addTheme("default");
            pManGUI->setCurrentTheme("default");
            pManGUI->loadAllThemes();
            
            // Text fonts
            Nexus::ManagerTextFonts* pManTextFonts = Nexus::ManagerTextFonts::getPointer();
            pManTextFonts->loadAll();
            
            // Sprite manager
            Nexus::ManagerSprites* pManSprites = Nexus::ManagerSprites::getPointer();

            // Initialise all applications
            Nexus::ManagerApplications* pManApplications = Nexus::ManagerApplications::getPointer();
            std::string applicationName = "Development";
            Nexus::ApplicationDevelopment applicationDevelopment(applicationName);
            pManApplications->callAllApps_initOnce();

            // Load all textures
            pManTextures->loadGroup("default");
            pManTextures->loadGroup("fonts");

 //           Nexus::TextFont *pTextFont = pManTextFonts->addTextFont("fonts/publicsans_16");
            pManTextFonts->loadAll();

            // Main loop
            while (pRD->updateWindow())
            {
                timing.update();

                pManInputDevices->update(pRD->getWindowFullscreen(), pRD->getWindowWidth(), pRD->getWindowHeight());
                if (!pManApplications->callCurrentApp_onUpdate())
                    break;

                pManSprites->update();
                pManSprites->render();
                pManGUI->update();
                pManGUI->render();
 
                pRD->swapBuffers();
                Sleep(0);
            }
            pManInputDevices->shutdown();
            pRD->closeWindow();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::string error = e.what();
        Nexus::Log::getPointer()->add(error, true);
        MessageBox(NULL, error.c_str(), "Exception...", MB_OK);
        
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
