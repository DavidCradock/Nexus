![Nexus project logo. Just a font rendered with the text "NeXus" within the 3D application Blender.](https://github.com/DavidCradock/Nexus/blob/4b3d00e3a4ae0950816c73e66e67e34823163bb7/textures/github_social_image.png)
# My current personal project.
## Written in C++ and testing on Windows 11, using OpenGL core profile for rendering.
##Features
- AI Neural network with genetic algorithm training. Some old code I wrote back in around 2002.
- 2D sprite rendering on the GPU
- Font rendering with data created from truetype fonts which are installed on the OS and then saved to a pair of files containing character information and each character's image inside a texture, so no need to install any fonts on the target machine and even when not rendered to a buffer for faster re-rendering, I get almost 10,000 fps when rendering text.
- Input from keyboard, joystick and mouse (Will be re-written in places, especially the joystick code which currently is really basic. Uses DirectX 8.
- The beginnings of a 2D rendered GUI. Currently has a window container object which holds and deals with all other GUI objects such as buttons, text edit boxes etc.

##TODO

Add more AI stuff Steering, pathfinding, finite state machines and ?
Audio loading/saving/playback of .wav and .ogg
Add additional save formats to Image
Implement archiveManager to all
Add logging all over the place
Move each manager into their own files
Unify each manager so they each have reference counts and maybe groups?
Add a loading screen mechanism of some kind.
Remove all the old coding style of underscores etc. Tidy up all old code with new.
Check RenderTexture code.
Add line rendering.
###Add more GUI objects.
Generic buttons, image buttons, sliders, progress bars, text, listbox, scroll bars, render texture, tabs, graphs for statistics, static and animated images(sprites), text edit boxes, generic file requestor load/save pre-built windows and statistics (Framerate, CPU, RAMM etc) pre-built window.
