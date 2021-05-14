@echo off
echo Clean up compilations
echo .

attrib -h ".vs"

rmdir /q/s Debug
rmdir /q/s Release
rmdir /q/s x64
rmdir /q/s bin_Debugx64
rmdir /q/s bin_Debugx86
rmdir /q/s bin_Releasex64
rmdir /q/s bin_Releasex86
rmdir /q/s Installer\Debug
rmdir /q/s Installer\Release
rmdir /q/s Installer\x64
rmdir /q/s QueryTool\Debug
rmdir /q/s QueryTool\Release
rmdir /q/s QueryTool\x64
rmdir /q/s ReferenceGuide\Debug
rmdir /q/s ReferenceGuide\Release
rmdir /q/s ReferenceGuide\x64
rmdir /q/s SQLComponents\Debug
rmdir /q/s SQLComponents\Release
rmdir /q/s SQLComponents\x64
rmdir /q/s StyleFramework\Debug
rmdir /q/s StyleFramework\Release
rmdir /q/s StyleFramework\x64
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