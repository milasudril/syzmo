dim filesys
set filesys=CreateObject("Scripting.FileSystemObject")
If filesys.FolderExists("C:\syzmo\client_cgi") Then
	filesys.CopyFolder "C:\syzmo\client_cgi", "C:\Inetpub\wwwroot", true
End If

If filesys.FolderExists("C:\syzmo\server") Then
	filesys.CopyFolder "C:\syzmo\server", "C:\Program\syzmo", true
End If

dim shell
set shell = WScript.CreateObject("WScript.Shell")
shell.Run "C:\Program\syzmo\syzmo_server.exe", 0, false
