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
rmdir /q/s bin_DebugUnicodex86
rmdir /q/s bin_DebugUnicodex64
rmdir /q/s bin_ReleaseUnicodeWin32
rmdir /q/s bin_ReleaseUnicodex64
rmdir /q/s Installer\Debug
rmdir /q/s Installer\Release
rmdir /q/s Installer\DebugUnicode
rmdir /q/s Installer\ReleaseUnicode
rmdir /q/s Installer\x64
rmdir /q/s QueryTool\Debug
rmdir /q/s QueryTool\Release
rmdir /q/s QueryTool\DebugUnicode
rmdir /q/s QueryTool\ReleaseUnicode
rmdir /q/s QueryTool\x64
rmdir /q/s ReferenceGuide\Debug
rmdir /q/s ReferenceGuide\Release
rmdir /q/s ReferenceGuide\DebugUnicode
rmdir /q/s ReferenceGuide\ReleaseUnicode
rmdir /q/s ReferenceGuide\x64
rmdir /q/s SQLComponents\Debug
rmdir /q/s SQLComponents\DebugUnicode
rmdir /q/s SQLComponents\Release
rmdir /q/s SQLComponents\ReleaseUnicode
rmdir /q/s SQLComponents\x64
rmdir /q/s SQLMigrate\Debug
rmdir /q/s SQLMigrate\Release
rmdir /q/s SQLMigrate\DebugUnicode
rmdir /q/s SQLMigrate\ReleaseUnicode
rmdir /q/s SQLMigrate\x64
rmdir /q/s StyleFramework\Debug
rmdir /q/s StyleFramework\Release
rmdir /q/s StyleFramework\DebugUnicode
rmdir /q/s StyleFramework\ReleaseUnicode
rmdir /q/s StyleFramework\x64
rmdir /q/s BaseLibrary\Debug
rmdir /q/s BaseLibrary\Release
rmdir /q/s BaseLibrary\x64
rmdir /q/s BaseLibrary\Win32
rmdir /q/s QueryRewriter\Debug
rmdir /q/s QueryRewriter\Release
rmdir /q/s QueryRewriter\DebugUnicode
rmdir /q/s QueryRewriter\ReleaseUnicode
rmdir /q/s QueryRewriter\x64
rmdir /q/s DebugUnicode
rmdir /q/s lib
rmdir /q/s ".vs"

del ReferenceGuide\*.chm
del ReferenceGuide\*.backup
del Installer\Installer_32bits\*.exe
del Installer\Installer_64bits\*.exe

del /s /f *.ilk
del /s /f *.ncb
del /s /f *.aps
del /s /f *.plg
del /s /f *.chm
del /s /f *.exe
del /s /f *.user
del /s /f *.sdf
del /s /f *.pdb
del /s /f *.VC.db

echo .
echo Done cleaning compilations
pause