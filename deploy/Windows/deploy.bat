if exist package rd /s /q package
mkdir package
copy ..\..\bin\release\release\FreelanceNavigator.exe package\FreelanceNavigator.exe
c:\Qt\Qt5.6.0\5.6\msvc2015\bin\windeployqt.exe package\FreelanceNavigator.exe
copy redist\*.* package\
copy libeay32.dll package\libeay32.dll
copy ssleay32.dll package\ssleay32.dll