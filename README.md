SyZmO - SYnthesiZer reMOte
===========================
This is a client-server solution for using a synthesizer hosted by another machine. This is achieved by communicating with the server providing the synthesizer via UDP messages.

SyZmO consists of three programs:

 * `syzmo_client_studio` is the client directly visible to the end user. Its purpose is to act as a regular MIDI client in the music production environment
 * `syzmo_client_cgi` is a client running as a `cgi` program. This makes it possible to control the server via a web interface.
 * `syzmo_server` is the server communicating with the synthesizer

Dependencies
------------
`syzmo_client_studio`
.....................
 * gabi
 * mustudio
 * The `nanosleep` function
 
 `syzmo_server`
 ..............
  * A C++03 compiler
  * Windows 98 or later
  
 `syzmo_client_cgi`
 ..............
  * A C++03 compiler
  * Windows 98 or later
  * A web server that works on the server system

The server-side code, `syzmo_client_cgi` and `syzmo_server`, only relies on C++03 and Windows API. The reason for this is that they are is designed to work on Windows 98 with the use of a suitable compiler, and `TDM-GCC 4.8.1`, required to compile Gabi, seems to produce incompatible binaries, that without `-municode` and with `-m32`. If you are lucky and have a *complete* driver for your audio hardware that works on a newer Windows box, there should not be any problems. In the case Windows 98 is needed, `MinGW 3.4.5` is a compiler that produces working binaries and also runs on the server.

To host the web interface, a web server is needed. On Windows 98, one possible choice is the Personal Web Server, distributed with the system.

TODO:s
------
 * Fix controlled shutdown on both client and server
 * Implement proper exception handling
 * Complete the web interface
 * Write a quick instruction manual

