# Auravyx (C++ Voxel Game)
---


![Alt Text](banner.gif)

---

### Welcome to Auravyx! Build whatever you can imagine with friends or on your own in this smooth voxel based sandbox. Easily moddable too!

---

- [**Installation**](#installation)
  * [Download](#download)
  * [Change settings](#change-settings)
  * [Troubleshooting](#troubleshooting)
- [**Getting Started**](#installation)
  * [Controls](#controls)
  * [Loading screen](#loading-screen)
  * [Main menu](#troubleshooting)
  * [Debugging](#debugging)
- [**Server Guide**](#server-guide)
  * [Setting up a server](#setting-up-a-server)
  * [Connecting to a server](#connecting-to-a-server)
  * [Using server console](#using-server-console)
  * [Connecting to an internet server](#connecting-to-an-internet-server)
- [**Modding Guide**](#modding-guide)
  * [Installing mods](#installing-mods)
  * [Creating your own mod](#creating-your-own-mod)
    + [Setting up development environment](#setting-up-development-environment)
    + [Creating basic mod](#creating-basic-mod)
    + [Custom resources](#temp)
    + [Custom shaders](#temp)
    + [Custom server](#temp)
    + [External documentation](#external-documentation)
- [**Gallery**](#gallery)
  * [Pictures](#pictures)
  * [Videos](#videos)


<br>

## Installation

#### Download


[Download latest .zip](https://github.com/Voxol/Voxel/releases/latest)

Extract .zip anywhere you'd like and run the Auravyx.exe


---

#### Change settings

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

#### Troubleshooting

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

<br>

## Getting Started

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

#### Controls

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

#### Loading screen

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

#### Main menu

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

#### Debugging

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,

---

<br>

## Server Guide

#### Setting up a server

#### Connecting to a server

#### Using server console

#### Connecting to an internet server

<br>

## Modding Guide

#### Installing mods

---

#### Creatng your own mod

This is mod.

---

##### Setting up development environment

##### Creating basic mod

##### External documentation

<br>

## Gallery

#### Pictures

![Imgur](https://imgur.com/K9W9gjf.jpg)


![Image](https://imgur.com/r8ZHfNG.jpg)


![Image](https://imgur.com/xg8gjfI.jpg)


![Image](https://imgur.com/uXEN1i6.jpg)

![Image](https://imgur.com/2poeBib.jpg)

---

#### Videos

---

Connecting to a server on the other side of the globe is just as easy as connecting to your own local server. 

Since the current version of the server runs only on Windows, hosting must be done on Windows. Don't worry, a Linux version will come soon. I was able to get it running on a AWS Windows Server 2016 server. I did have to allow some port usuage since it was disabled by default, but I was able to connect.




A C++ implementation of my Java Smooth Voxels, fully destructable voxel sandbox with multiplayer, also a lot quicker too. Still in development. Runs on Windows, unconfirmed if it can run on Linux/Mac (though it should be able to)

Current version has a few things disabled (chunk streaming, force noclip, multiplayer disabled). Some features are being reimplemented to be handled server-side.

Most up-to-date version: See Releases

How to play: Extract lastest release zip folder and run the .exe, if this fails make sure anti-virus are ignoring it (Windows Defender may try to auto-delete)

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

Connecting to a server
---------------

All sessions opperate on a server, even singleplayer, this allows you to play via lan or host on a server. If you want to host a seperate game from a server, run ServerHost.exe. This will give you a complete server operating experience, allowing you to view connected players and ping as well as a log of chat and commands.

To disable self hosting, change the line in Settings.txt "host=true" to "host=false" and the IP to the server's IP.

Port 54000 should work on most computers. If port binding fails, try another (make sure to change settings.txt and server's port)

