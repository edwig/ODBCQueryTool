@echo off
echo Clean up compilations
echo .

attrib -h ".vs"

rmdir /q/s Debug
rmdir /q/s Release
rmdir /q/s x64
rmdir /q/s Src\SQLComponents\Debug
rmdir /q/s Src\SQLComponents\Release
rmdir /q/s Src\SQLComponents\x64
rmdir /q/s ipch
rmdir /q/s lib
rmdir /q/s ".vs"

del ReferenceGuide\*.chm
del ReferenceGuide\*.backup
del Installer\Installer_32bits\*.exe
del Installer\Installer_64bits\*.exe

del *.ilk
del *.ncb
del *.aps
del *.plg
del *.chm
del *.exe
del *.user
del *.sdf
del *.pdb
del *.VC.db

echo .
echo Done cleaning compilations
pause