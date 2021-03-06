' This script copies files from the shared folder "C:\syzmo" to appropriate
' places.
'
' To get the www interface working in Personal Web Server:
'  * Make sure that <Home> has the permissions "read", "execute", and "script".
'  * Also set the default document to "syzmo_client_cgi.exe?view=info".

On Error Resume Next

dim filesys
set filesys=CreateObject("Scripting.FileSystemObject")
If filesys.FolderExists("C:\syzmo\client_cgi") Then
' This will copy CGI files to the wwwroot directory.
	filesys.CopyFolder "C:\syzmo\client_cgi", "C:\Inetpub\wwwroot", true
End If

If filesys.FolderExists("C:\syzmo\server") Then
' Copy the server
	filesys.CopyFolder "C:\syzmo\server", "C:\Program\syzmo", true
End If

dim shell
set shell = WScript.CreateObject("WScript.Shell")
' Start the server. For debugging, it is useful to show any windows opened by
' the server process.
shell.Run "C:\Program\syzmo\syzmo_server.exe", 4, false
