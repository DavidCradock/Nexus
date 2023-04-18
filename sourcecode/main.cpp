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
            Nexus::InputManager* pIM = Nexus::InputManager::getPointer();
            pIM->init(pRD->getWindowHandle());

            // Timing
            Nexus::Timing timing;
            timing.setStatFPSSrate(5);

            // Shaders
            Nexus::ShaderManager* pSM = Nexus::ShaderManager::getPointer();
            pSM->addNewGroup("default");
            pSM->addShader("sprites", "shaders/simple.vs", "shaders/simple.fs", "default");
            pSM->addShader("sprites", "shaders/sprites.vs", "shaders/sprites.fs", "default");
            pSM->addShader("textFont", "shaders/textFont.vs", "shaders/textFont.fs", "default");
            pSM->addShader("gui", "shaders/gui.vs", "shaders/gui.fs", "default");
            pSM->loadGroup("default");

            // Texture manager groups
            Nexus::TextureManager* pTM = Nexus::TextureManager::getPointer();
            pTM->addNewGroup("default");     

            // GUI
            Nexus::GUIManager* pGUI = Nexus::GUIManager::getPointer();
            // Create default theme and set as default
            Nexus::GUITheme* pTheme = pGUI->createTheme("default");
            pGUI->setCurrentTheme("default");
            pGUI->loadAllThemes();
            
            // Text fonts
            Nexus::TextFontManager* pTFM = Nexus::TextFontManager::getPointer();
            pTFM->loadAll();
            
            // Sprite manager
            Nexus::SpriteManager* pSpriteMan = Nexus::SpriteManager::getPointer();

            // Initialise all applications
            Nexus::ApplicationManager* pAM = Nexus::ApplicationManager::getPointer();
            std::string applicationName = "Development";
            Nexus::ApplicationDevelopment applicationDevelopment(applicationName);
            pAM->callAllApps_initOnce();

            // Load all textures
            pTM->loadGroup("default");
            pTM->loadGroup("fonts");

            Nexus::TextFont *pTextFont = pTFM->addTextFont("fonts/publicsans_16");
            pTFM->loadAll();

            // Make sure the "fonts" group exists in the texture manager, if not, create it
            //TextureManager* pTM = TextureManager::getPointer();
            //if (!pTM->groupExists("fonts"))
            //   pTM->addNewGroup("fonts");

            // Main loop
            while (pRD->updateWindow())
            {
                timing.update();

                pIM->update(pRD->getWindowFullscreen(), pRD->getWindowWidth(), pRD->getWindowHeight());
                if (!pAM->callCurrentApp_onUpdate())
                    break;

                pSpriteMan->update();
                pSpriteMan->render();
                pGUI->render();
                std::string strFramerate("Framerate: ");
                strFramerate.append(std::to_string(timing.getStatFPSS()));
                strFramerate.append(" frames per second");
                pTextFont->print(strFramerate, 0, 0, Nexus::CColouruc(255, 255, 255, 255));
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
        Nexus::Log::getPointer()->add(error, true);
        MessageBox(NULL, error.c_str(), "Exception...", MB_OK);
        
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
