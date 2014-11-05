SyZmO - SYnthesiZer reMOte
===========================
This is a client-server solution for using a synthesizer hosted by another machine. This is achieved by communicating with the server providing the synthesizer via UDP messages.

SyZmO consists of three programs:

 * `syzmo_client_studio` is the client directly visible to the end user. Its purpose is to act as a regular MIDI client in the music production environment
 * `syzmo_client_cgi` is a client running as a `cgi` program. This makes it possible to control the server via a web interface.
 * `syzmo_server` is the server communicating with the synthesizer

Dependencies
------------
For compiling the "studio client", the following packages are needed:

 * gabi
 * mustudio

The compilation process is controlled by `wand` (part of gabi).

The two other programs is designed to work on Windows 98 with the use of a suitable compiler. The reason for supporting Windows 98 as a server operating system is the lack of complete drivers for old audio hardware on other operating systems. In the case Windows 98 is needed, `MinGW 3.4.5` is a compiler that runs on the server and produces working binaries.

To host the web interface, a web server is needed. On Windows 98, one possible choice is the Personal Web Server, distributed with the system.

TODO:s
------
 * Fix controlled shutdown on both client and server
 * Implement proper exception handling
 * Complete the web interface
 * Write a quick instruction manual

