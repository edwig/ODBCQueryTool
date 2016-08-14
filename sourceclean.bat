@echo off
echo Clean up compilations
echo .

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
echo Done cleaning compilations
pause