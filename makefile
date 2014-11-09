all: syzmo_client_studio syzmo_client_cgi.exe syzmo_server.exe
.PHONY: syzmo_client_studio syzmo_client_cgi.exe syzmo_server.exe

__mingw:
	wget http://gdurl.com/RVgH -O __mingw.tar.gz
	gzip -d __mingw.tar.gz
	tar -xf __mingw.tar
	rm __mingw.tar

syzmo_client_studio:
	wand 'profile[release]'

syzmo_client_cgi.exe: __mingw
	wand 'platforms[target[x86;Windows]] profile[release]'

syzmo_server.exe: __mingw
	wand 'platforms[target[x86;Windows]] profile[release]'
