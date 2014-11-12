#!/bin/bash

echo -n "

This is SyZmO Server pre-install/upgrade script

Enter username for the SMB server $SYZMO_SERVER: "
read tmp
echo "username="$tmp > __samba_logon.txt
chmod 600 __samba_logon.txt
echo -n "Enter password for the SMB server $SYZMO_SERVER: "
read -s tmp
echo "password="$tmp >> __samba_logon.txt
unset tmp
smbclient '\\'$SYZMO_SERVER'\SYZMO' -A __samba_logon.txt << SMBSESSION
prompt
put syzmo_upgrade.vbs syzmo_upgrade.vbs
mkdir client_cgi
cd client_cgi
lcd client_cgi
mput *.css
mput *.html
mput syzmo.svg
mput lines.png
lcd ..
put __wand_targets_rel_win/syzmo_client_cgi.exe syzmo_client_cgi.exe
cd ..

cd server
put __wand_targets_rel_win/syzmo_server.exe syzmo_server.exe
logoff
exit
SMBSESSION
rm __samba_logon.txt
