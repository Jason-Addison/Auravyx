# C++ Smooth-Voxels
A C++ implementation of my Java Smooth Voxels, also a lot quicker too. Still in development. Runs on Windows, unconfirmed if it can run on Linux/Mac (though it should be able to)

Current version has a few things disabled (chunk streaming, force noclip, multiplayer disabled). Some features are being reimplemented to be handled server-side.

Most up-to-date version under x64/Release

->Since chunk streaming is disabled, I included a small (2gb) world, which loads on startup. There are some snowy mountains in the distance, a tunnel at spawn and a brick pillar coming out of the ground.

Visit my site if you want to chat: www.jasonaddison.dev

Default Controls (edit controls.txt to change)
---------------
WASD - Movement

T - Chat/Commands

R - Wireframe

-/+ - Time back/forward


F1 - Screenshot (saves to [exe dir]/Screenshots

F3 - Extra debug info / View color buffers

F5 - View chunk metrics

-Disabled for now-

Left-click - creates sphere of material at crosshair raycast position. ID is set by /id and size is set by /size

Right-click - removes an area defined by size

Commands
--------
/fps [integer]    Sets max fps

/fov [double]     Sets field of view

/speed [double]   Sets speed multiplier

/save             Saves currently running world to disk (if chunk streaming is off)

/noclip           Toggles terrain/object collision


Gallery
---------

![Imgur](https://imgur.com/K9W9gjf.jpg)


![Image](https://imgur.com/r8ZHfNG.jpg)


![Image](https://imgur.com/xg8gjfI.jpg)


![Image](https://imgur.com/uXEN1i6.jpg)

