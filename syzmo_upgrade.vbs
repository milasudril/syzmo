' This script copies files from the shared folder "C:\syzmo" to appropriate
' places.

dim filesys
set filesys=CreateObject("Scripting.FileSystemObject")
If filesys.FolderExists("C:\syzmo\client_cgi") Then
' This will copy CGI files to the wwwroot directory. In Personal Web Server:
' Make sure that <Home> has the permissions "read", "execute", and "script".
' Also set the default document to "syzmo_server_client.exe?view=info".
	filesys.CopyFolder "C:\syzmo\client_cgi", "C:\Inetpub\wwwroot", true
End If

If filesys.FolderExists("C:\syzmo\server") Then
' Copy the server
	filesys.CopyFolder "C:\syzmo\server", "C:\Program\syzmo", true
End If

dim shell
set shell = WScript.CreateObject("WScript.Shell")
' Start the server
shell.Run "C:\Program\syzmo\syzmo_server.exe", 0, false
