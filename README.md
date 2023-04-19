![Nexus project logo. Just some text "NeXus" extruded slightly and placed against a quad and the awesome Nishita sky setup within the 3D application Blender to give a natural, outdoors in sunset lighting.](https://github.com/DavidCradock/Nexus/blob/fd9d367a4d39ce8b52feb9a1b179e5446aef6944/git_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL core profile for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and it feels like this NeXus project could develop into a nice little 2D (and then later 3D too) game development framework.
**Current features**

- AI Neural network with genetic algorithm training. Some old code I wrote back in around 2002. This code is on the CPU only, no GPU and is singlethreaded as it was before a time when multiple cores were common. I will re-write this.
- 2D sprite rendering on the GPU. Needs checking.
- Font rendering with data created from truetype fonts which are installed on the OS and then saved to a pair of files containing character information and each character's image inside a texture, so no need to install any fonts on the target machine and even when not rendered to a buffer for faster re-rendering, I get almost 10,000 fps when rendering text.
- Input from keyboard, joystick and mouse (Will be re-written in places, especially the joystick code which currently is really basic. Uses DirectX 8.
- The beginnings of a 2D rendered GUI. Currently has a window container object which holds and deals with all other GUI objects such as buttons, text edit boxes etc.
![Screenshot of Nexus running in a window which shows text rendering and a couple of windows with their titlebar text reading Test Window1 and Test Window2.](https://github.com/DavidCradock/Nexus/blob/bf2bbfdd9524f6150c3152bedb34e96715ae0dd8/git_images/current_state.jpg)

**To do**


- Unify each manager so they each have reference counts and maybe groups?
- Go through each files' include section and make sure only the minimal is included due to manager change.
- Remove all the old coding style of underscores etc. Tidy up all the code with new.
- Check all the /graphics/sprites/ code and test the hell out of it.
- Check RenderTexture code.
- Add more GUI objects. Generic buttons, image buttons, sliders, progress bars, text, listbox, scroll bars, render texture, tabs, graphs for statistics, static and animated images(sprites), text edit boxes, generic file requestor load/save pre-built windows and statistics (Framerate, CPU, RAMM etc) pre-built window.
- Implement archiveManager to all classes which load from files
- Add line rendering.
- Add a loading screen mechanism of some kind.
- Add additional save formats to Image
- Add logging all over the place
- Add 2D particle systems with emitters, forces, physics etc, ala Blender.
- Rewrite AI neural network and genetic algorithm training classes to be multithreaded and perhaps also able to run on GPU?
- Rewrite Input stuff, especially the joystick part. We need to support multiple devices with multiple axis and number of buttons and triggers etc (XBox controller). I wishh I could get my hands on a forcefeedback joystick. Remapping of controls needed.
- Add more AI stuff. Steering, pathfinding, finite state machines and ?
- Add some kinda 2D map
- Add 3D octree based renderer using diffuse, emission, roughness, metalic, normal bumpmap and? textures.
- Audio loading/saving/playback of .wav and .ogg
- Add physics. Both 2D and 3D. Use pre-existing or write from scratch?
- Add Network helper code. Maybe use Raknet or whatever is out there these days?

**Done**
- Move each manager into their own files then move them to new filter "managers" and rename each class from ObjectManager to ManagerObjects
