@pushd "%~dp0..\.."
@call generate_projects_vc14xboxone.bat
@popd
@if %errorlevel% NEQ 0 exit /b 1

