@echo off
@echo Opschonen WinHTTP directories
@echo .

set VERSION=1.4.0

del *.rar
rmdir /q/s Debug
rmdir /q/s Release

rmdir /q/s x64
rmdir /q/s Installer\Installer_32bits\Debug
rmdir /q/s Installer\Installer_32bits\Release
rmdir /q/s Installer\Installer_64bits\Debug
rmdir /q/s Installer\Installer_64bits\Release
rmdir /q/s ipch

del ReferenceGuide\*.chm
del ReferenceGuide\*.backup

del *.ilk
del *.ncb
del *.aps
del *.plg
del *.chm
del *.exe
del *.user
del *.sdf
del *.pdb
echo .
echo Klaar met opschonen
echo .
echo maak een winrar bestand aan
echo .
"C:\Program Files\Winrar\rar.exe" a ODBCQuerytool_%VERSION%.rar ODBC*.* Data Dict Installer Misc ODBCQueryTool ReferenceGuide Screenshots Src Testfiles *.txt *.bat *.htm *.xml *.sql *.rtf
echo .
echo Klaar met RAR aanmaken
echo .
robocopy . C:\Users\%USERNAME%\OneDrive\Documenten ODBCQuerytool_%VERSION%.rar /xo
echo .
echo Klaar met archief naar de cloud kopieren
pause