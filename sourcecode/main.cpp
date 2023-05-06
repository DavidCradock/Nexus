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
            Nexus::InputDeviceManager* pManInputDevices = Nexus::InputDeviceManager::getPointer();
            pManInputDevices->init(pRD->getWindowHandle());

            // Timing
            Nexus::Timing timing;
            timing.setStatFPSSrate(1);

            // Shaders
            Nexus::ShaderManager* pManShaders = Nexus::ShaderManager::getPointer();
            pManShaders->addNewGroup("default");
            pManShaders->addShader("default", "shaders/default.vs", "shaders/default.fs", "default");
            pManShaders->loadGroup("default");

            // Texture manager groups
            Nexus::TextureManager* pManTextures = Nexus::TextureManager::getPointer();
            pManTextures->addNewGroup("default");
            pManTextures->add2DTexture("white_32x32", "textures/white_32x32.png", "default");

            // GUI
            Nexus::GUIManager* pManGUI = Nexus::GUIManager::getPointer();
            // Create default theme and set as default
            Nexus::GUITheme* pTheme = pManGUI->addTheme("default");
            pManGUI->setCurrentTheme("default");
            pManGUI->loadAllThemes();
            
            // Text fonts
            Nexus::TextFontManager* pManTextFonts = Nexus::TextFontManager::getPointer();
            pManTextFonts->loadAll();
            
            // Sprite manager
            Nexus::SpriteManager* pManSprites = Nexus::SpriteManager::getPointer();

            // Audio manager
            Nexus::AudioManager* pManAudio = Nexus::AudioManager::getPointer();

            // Initialise all applications
            Nexus::ApplicationManager* pManApplications = Nexus::ApplicationManager::getPointer();
            std::string applicationName = "Development";
            Nexus::ApplicationDevelopment applicationDevelopment(applicationName);
            pManApplications->callAllApps_initOnce();

            // Load all textures
            pManTextures->loadGroup("default");
            pManTextures->loadGroup("fonts");

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
        __debugbreak();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
