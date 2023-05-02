#include "precompiled_header.h"
#include "Nexus.h"

/* Example threading code

#include <mutex>
#include <thread>

std::mutex mtx;

void print_hello() {
  std::unique_lock<std::mutex> lock(mtx);
  std::cout << "Hello, world!" << std::endl;
  lock.unlock();
}

int main() {
  std::thread t1(print_hello);
  std::thread t2(print_hello);
  t1.join();
  t2.join();
  return 0;
}
*/

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
            pManShaders->addShader("default", "shaders/default.vs", "shaders/default.fs", "default");
            pManShaders->loadGroup("default");

            // Texture manager groups
            Nexus::ManagerTextures* pManTextures = Nexus::ManagerTextures::getPointer();
            pManTextures->addNewGroup("default");
            pManTextures->add2DTexture("white_32x32", "textures/white_32x32.png", "default");

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

            // Audio manager
            Nexus::ManagerAudio* pManAudio = Nexus::ManagerAudio::getPointer();
            pManAudio->addSample("audio/38_Introductory.wav");
            pManAudio->loadAll();
            pManAudio->playSample("audio/38_Introductory.wav");
            Sleep(200);
            pManAudio->playSample("audio/38_Introductory.wav");

            // Initialise all applications
            Nexus::ManagerApplications* pManApplications = Nexus::ManagerApplications::getPointer();
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
