SyZmO - SYnthesiZer reMOte
===========================
This is a client-server solution for using a synthesizer hosted by another machine. This is achieved by communicating with the server providing the synthesizer via UDP messages.

SyZmO consists of three programs:

 * `syzmo_client_studio` is the client directly visible to the end user. Its purpose is to act as a regular MIDI client in the music production environment
 * `syzmo_client_cgi` is a client running as a `cgi` program. This makes it possible to control the server via a web interface.
 * `syzmo_server` is the server communicating with the synthesizer

Dependencies
------------
### Build process
 * wand
 * Samba client
 * wget
 * wine

### `syzmo_client_studio`
 * gabi
 * mustudio
 * POSIX

### `syzmo_server`
 * Windows API

### `syzmo_client_cgi`
 * Windows API
 * A web server with CGI support that works on the server system

### Details
The client should compile and run on a GNU/Linux system with gcc 4.8.

The server-side code needs the following dlls:
 * `winmm.dll`
 * `shell32.dll` version 4.71 or later

The server-side code, `syzmo_client_cgi` and `syzmo_server` are is designed to work on Windows 98. The reason for this is driver compatiblity. This implies some restrictions on what compiler can be used to produce binaries. `TDM-GCC 4.8.1`, required to compile Gabi, seems to produce incompatible binaries, that without `-municode` and with `-m32`. If you are lucky and have a *complete* driver for your audio hardware that works on a newer Windows box, there should not be any problems. Also, if you have such drivers for Linux, then the server code should work well in Wine. In the case Windows 98 is needed, `MinGW 3.4.5` is a compiler that produces working binaries and also runs on the server.

To host the web interface, a web server is needed. On Windows 98, one possible choice is the Personal Web Server, distributed with the system.

Special privileges required for the server
------------------------------------------
*Those using Windows 98 as operating system on the server can skip this section*

To be able to use the `ExitWindowsEx`function, the server has started by an "interactive user". This user needs the following permissions:

 * Access the MIDI hardware through the `midiOutXXX` functions
 * Bind a network port
 * Shutdown and reboot the system

Since an interactive user is needed, auto-login has to be enabled.

Installing the system
---------------------
*This section assumes that the server is running on a Windows desktop*

 1. Create a network share on the server machine. In this instruction, the directory `C:\syzmo` will be used as network share `\\syzmo_server\syzmo`.
 2. Run `make variables` to get a list of variables that affects the installation process.
 3. Set the environment variables listed appropriately. If you wand to upgrade the system, it is a good idea to add these to your `.profile` or `.bashrc` file.
 4. Run `make install` from this directory. The makefile will download a binary distribution of `MinGW 3.4.5`, that will be used to compile the server code.
 5. When the script asks for login information, type the username, and password. If there is no password just hit ENTER to skip to the next question.
 6. On the server machine, create a shortcut to `C:\syzmo\syzmo_upgrade.vbs` in the `Autostart` folder in the "Start" menu. For more information, you may have a look at this file in `notepad`.
 7. Write configuration files similar to `syzmo_config_*`. They should be stored in a corresponding `%APPDATA%` directory. On Linux, the user's home directory is used. On Windows, it is the `Application Data` directory.


TODO:s
------
 * Fix controlled shutdown on both client and server
 * Implement proper exception handling
 * Complete the web interface
 * Improve quick installation guide

