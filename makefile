all: syzmo_client_studio syzmo_client_cgi.exe syzmo_server.exe
.PHONY: syzmo_client_studio syzmo_client_cgi.exe syzmo_server.exe install\
variables

VARS_OLD := $(.VARIABLES)

#This url shall refer to a tar.gz file containing the MinGW 3.4.5 binary
#distribution
SYZMO_MINGW_URL?=http://gdurl.com/RVgH

#This is the name of the host that runs the server
SYZMO_HOST?=syzmo_host
#This is the name of a test host that runs the server
SYZMO_HOST_TEST?=syzmo_host_test

variables:
	$(foreach v,                                        \
	$(filter-out $(VARS_OLD) VARS_OLD,$(.VARIABLES)), \
	$(info $(v) = $($(v))))

__mingw:
	wget $(SYZMO_MINGW_URL) -O __mingw.tar.gz
	gzip -d __mingw.tar.gz
	tar -xf __mingw.tar
	rm __mingw.tar

syzmo_client_studio:
	wand 'profile[release]'

syzmo_client_cgi.exe: __mingw
	wand 'platforms[target[x86;Windows]] profile[release]'

syzmo_server.exe: __mingw
	wand 'platforms[target[x86;Windows]] profile[release]'

install: all
	wand 'profile[install]'
	SYZMO_HOST=$(SYZMO_HOST) ./server_install.sh

install-test: syzmo_client_cgi.exe syzmo_server.exe
	SYZMO_HOST=$(SYZMO_HOST_TEST) ./server_install.sh

install-server: syzmo_client_cgi.exe syzmo_server.exe
	SYZMO_HOST=$(SYZMO_HOST) ./server_install.sh
