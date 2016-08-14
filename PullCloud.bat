@echo off
echo OPEN-ODBCQuerytool uit de cloud halen
echo .

set VERSION=1.4.0

echo Opschonen WinHTTP directories
echo .

rmdir /q/s Debug
rmdir /q/s Release
rmdir /q/s x64
rmdir /q/s Installer\Installer_32bits\Debug
rmdir /q/s Installer\Installer_32bits\Release
rmdir /q/s Installer\Installer_64bits\Debug
rmdir /q/s Installer\Installer_64bits\Release
rmdir /q/s ipch
rmdir /q/s Data 
rmdir /q/s Dict 
rmdir /q/s Installer 
rmdir /q/s Misc 
rmdir /q/s ODBCQueryTool 
rmdir /q/s ReferenceGuide 
rmdir /q/s Screenshots
rmdir /q/s Src 
rmdir /q/s Testfiles 

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
del ODBC*.*
del *.txt 
del *.htm 
del *.xml 
del *.sql 
del *.rtf

echo .
echo Klaar met opschonen
echo .
echo Haal bestand uit de cloud
robocopy C:\Users\%USERNAME%\OneDrive\Documenten . ODBCQuerytool_%VERSION%.rar /xo
echo .
echo Klaar met RAR kopieren
echo .
"C:\Program Files\Winrar\rar.exe" x -o+ ODBCQuerytool_%VERSION%.rar
echo .
echo Klaar met archief uit de cloud halen
echo .
pause
