@echo off
:: Copies the latest MwLibrary to SDK.

set solutionFolderVar="..\"
set destFolder="C:\Programmering\SDK\mw"

del %destFolder%\include\mw\ /Q
del %destFolder%\include\mw\signals\ /Q
del %destFolder%\include\mw\enet\ /Q

del %destFolder%\source\ /Q
del %destFolder%\source\signals\ /Q
del %destFolder%\source\enet\ /Q

del %destFolder%\lib-r\ /Q
del %destFolder%\lib-d\ /Q

xcopy %solutionFolderVar%\MwLibrary\mw\*.h %destFolder%\include\mw\
xcopy %solutionFolderVar%\MwLibrary\mw\*.cpp %destFolder%\source\

xcopy %solutionFolderVar%\MwLibrary\mw\signals\*.h %destFolder%\include\mw\signals\
xcopy %solutionFolderVar%\MwLibrary\mw\signals\*.cpp %destFolder%\source\signals\

xcopy %solutionFolderVar%\MwLibrary\mw\enet\*.h %destFolder%\include\mw\enet\
xcopy %solutionFolderVar%\MwLibrary\mw\enet\*.cpp %destFolder%\source\enet\

xcopy %solutionFolderVar%\MwLibrary\Release\*.pdb %destFolder%\lib-r\
xcopy %solutionFolderVar%\Release\MwLibrary.lib %destFolder%\lib-r\
xcopy %solutionFolderVar%\MwLibrary\Debug\*.pdb %destFolder%\lib-d\
xcopy %solutionFolderVar%\MwLibrary\Debug\*.idb %destFolder%\lib-d\
xcopy %solutionFolderVar%\Debug\MwLibrary.lib %destFolder%\lib-d\

::"c:\Program Files\7-Zip\7z.exe" a "%destFolder%\mw.7z" "%destFolder%\"
