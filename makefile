all: __wand_targets_rel_lnx/client_studio/syzmo_client_studio \
__wand_targets_rel_win/client_cgi/syzmo_client_cgi.exe \
__wand_targets_rel_win/server/syzmo_server.exe

__mingw:
	wget http://gdurl.com/RVgH -O __mingw.tar.gz
	gzip -d __mingw.tar.gz
	tar -xf __mingw.tar
	rm __mingw.tar

__wand_targets_rel_lnx/client_studio/syzmo_client_studio:
	wand 'profile[release]'

__wand_targets_rel_win/client_cgi/syzmo_client_cgi.exe: __mingw
	wand 'platforms[target[x86;Windows]] profile[release]'

__wand_targets_rel_win/server/syzmo_server.exe: __mingw
	wand 'platforms[target[x86;Windows]] profile[release]'
