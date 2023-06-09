![Nexus project logo. Just some text "NeXus" extruded slightly and placed against a quad and the awesome Nishita sky setup within the 3D application Blender to give a natural, outdoors in sunset lighting.](https://github.com/DavidCradock/Nexus/blob/fd9d367a4d39ce8b52feb9a1b179e5446aef6944/git_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL core profile for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and it feels like this NeXus project could develop into a nice little game development framework.
**Current features**

- AI Neural network with genetic algorithm training. Some old code I wrote back in around 2002. This code is on the CPU only, no GPU and is singlethreaded as it was before a time when multiple cores were common. I will re-write this.
- 2D sprite rendering on the GPU. Needs checking.
- Font rendering with data created from truetype fonts which are installed on the OS and then saved to a pair of files containing character information and each character's image inside a texture, so no need to install any fonts on the target machine and even when not rendered to a buffer for faster re-rendering, I get almost 10,000 fps when rendering text.
- Input from keyboard, joystick and mouse (Will be re-written in places, especially the joystick code which currently is really basic. Uses DirectX 8.
- The beginnings of a 2D rendered GUI. Currently has a window container object which holds and deals with all other GUI objects such as buttons, text edit boxes etc.
![Screenshot of Nexus running in a window which shows text rendering and a couple of windows with their titlebar text reading Test Window1 and Test Window2.](https://github.com/DavidCradock/Nexus/blob/f0b82ace07ab3732a02455422e1dff1373dcac1b/git_images/current_state.jpg)

**To do*
- Add audio to GUI
- Test Matrix::setProjection() and setView() etc
- Fix the huge mess I've made with changing vertex buffer colour from RGB to RGBA
- Remove TextFonts usage of the texture manager and instead, have the texture object in the TextFont object?
- Unify each manager so they each have reference counts and maybe groups?
- Check all the /graphics/sprites/ code and test the hell out of it.
- Add more GUI objects. Image buttons, sliders, progress bars, listbox, scroll bars, render texture, tabs, graphs for statistics, static and animated images(sprites), text edit boxes, generic file requestor load/save pre-built windows and statistics (Framerate, CPU, RAMM etc) pre-built window.
- Implement archiveManager to all classes which load from files
- Add a loading screen mechanism of some kind.
- Add logging all over the place
- Add 2D particle systems with emitters, forces, physics etc, ala Blender.
- Rewrite AI neural network and genetic algorithm training classes to be multithreaded and perhaps also able to run on GPU?
- Rewrite Input stuff, especially the joystick part. We need to support multiple devices with multiple axis and number of buttons and triggers etc (XBox controller). I wish I could get my hands on a forcefeedback joystick. Remapping of controls needed.
- Add more AI stuff. Steering, pathfinding, finite state machines and ?
- Add some kinda 2D map
- Add 3D octree based renderer using diffuse, emission, roughness, metalic, normal bumpmap and? textures.
- Audio loading/saving/playback of .wav and .ogg
- Add physics. Both 2D and 3D. Use pre-existing or write from scratch?
- Add Network helper code. Maybe use Raknet or whatever is out there these days?

**Done**
- Add line rendering via GUILineGraph
- Fix rendering of windows so that their main centre area is acutally their set dimensions.
- Check RenderTexture code and fix TextFont print methods to accept render target dimensions
- Fix multiple windows being moved at the same time if their title bars are roughly in the same position.
- Moved updating and rendering of windows from GUIManager to GUIWindow
- Added Z ordering of GUI windows.
- Remove all the old coding style of underscores, C/S at beginning of class/structs, "_m" for private members etc. Tidy up all the code with new.
- Go through all code and standardize pointer names to all the managers
- Added saveAsJPG to Image
- Replace all Log::getPointer()->exception() call to Log::getPointer()->exception()
- Move each manager into their own files then move them to new filter "managers" and rename each class from ObjectManager to ManagerObjects
