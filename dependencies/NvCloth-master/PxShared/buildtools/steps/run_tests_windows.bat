@set BIN_PATH=%~dp0..\..\bin\vc14win64-cmake
@pushd "%BIN_PATH%"
@call BlastUnitTestsDEBUG_x64.exe --gtest_output=xml:BlastUnitTestsDEBUG_x64.xml
@echo ##teamcity[importData type='gtest' parseOutOfDate='true' file='%BIN_PATH%\BlastUnitTestsDEBUG_x64.xml']
@popd
@if %errorlevel% NEQ 0 exit /b 1