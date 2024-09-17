@echo off
setlocal enabledelayedexpansion

:: Define the paths to the template files
set "CMAKE_TEMPLATE_FILE=CMakeLists.txt.template"
set "CPP_TEMPLATE_FILE=App.cpp.template"

:: Check if the template files exist
if not exist "%CMAKE_TEMPLATE_FILE%" (
    echo [ERROR] Template file "%CMAKE_TEMPLATE_FILE%" not found.
    echo Please ensure "%CMAKE_TEMPLATE_FILE%" is in the same directory as this script.
    pause
    exit /b
)

if not exist "%CPP_TEMPLATE_FILE%" (
    echo [ERROR] Template file "%CPP_TEMPLATE_FILE%" not found.
    echo Please ensure "%CPP_TEMPLATE_FILE%" is in the same directory as this script.
    pause
    exit /b
)

:: Prompt the user to enter a project name
echo Please enter the new project name :
set /p project_name=

:: Check if the project name is empty
if "%project_name%"=="" (
    echo [ERROR] Project name cannot be empty.
    pause
    exit /b
)

:: Check if the project name contains invalid characters
echo %project_name% | findstr /r /c:"[\\/:*?""<>|]" >nul
if not errorlevel 1 (
    echo [ERROR] Project name contains invalid characters (\\ / : * ? " < > |).
    pause
    exit /b
)

:: Create the project directory
mkdir "%project_name%"
if errorlevel 1 (
    echo [ERROR] Failed to create directory "%project_name%". It may already exist.
    pause
    exit /b
)
echo [INFO] Directory "%project_name%" created successfully.

:: Create the 'src' subdirectory
mkdir "%project_name%\src"
if errorlevel 1 (
    echo [ERROR] Failed to create subdirectory "src" in "%project_name%".
    pause
    exit /b
)
echo [INFO] Subdirectory "src" created successfully.

:: Create the 'resources' subdirectory
mkdir "%project_name%\resources"
if errorlevel 1 (
    echo [ERROR] Failed to create subdirectory "resources" in "%project_name%".
    pause
    exit /b
)
echo [INFO] Subdirectory "resources" created successfully.

:: Copy and customize the CMakeLists.txt file
copy "%CMAKE_TEMPLATE_FILE%" "%project_name%\CMakeLists.txt" >nul
if errorlevel 1 (
    echo [ERROR] Failed to copy template file "%CMAKE_TEMPLATE_FILE%".
    pause
    exit /b
)
echo [INFO] Template file copied to "%project_name%\CMakeLists.txt".

:: Replace placeholders in CMakeLists.txt with the project name
powershell -Command ^
    "(Get-Content -Path '%CD%\%project_name%\CMakeLists.txt') -replace '__PROJECT_NAME__', '%project_name%' | Set-Content -Path '%CD%\%project_name%\CMakeLists.txt'"

:: Check if the replacement was successful
if errorlevel 1 (
    echo [ERROR] Failed to replace placeholders in CMakeLists.txt.
    pause
    exit /b
)
echo [INFO] CMakeLists.txt customized with project name "%project_name%".

:: Define the new C++ file name
set "NEW_CPP_FILE=%project_name%App.cpp"

:: Copy and customize the App.cpp file
copy "%CPP_TEMPLATE_FILE%" "%project_name%\src\%NEW_CPP_FILE%" >nul
if errorlevel 1 (
    echo [ERROR] Failed to copy template file "%CPP_TEMPLATE_FILE%".
    pause
    exit /b
)
echo [INFO] Template file copied to "%project_name%\src\%NEW_CPP_FILE%".

:: Replace placeholders in the new App.cpp with the project name
powershell -Command ^
    "(Get-Content -Path '%CD%\%project_name%\src\%NEW_CPP_FILE%') -replace '__PROJECT_NAME__', '%project_name%' | Set-Content -Path '%CD%\%project_name%\src\%NEW_CPP_FILE%'"

:: Check if the replacement was successful
if errorlevel 1 (
    echo [ERROR] Failed to replace placeholders in %NEW_CPP_FILE%.
    pause
    exit /b
)
echo [INFO] %NEW_CPP_FILE% customized with project name "%project_name%".

:: Append add_subdirectory(<ProjectName>) to the existing CMakeLists.txt in the batch script directory
echo add_subdirectory(%project_name%) >> CMakeLists.txt
if errorlevel 1 (
    echo [ERROR] Failed to append add_subdirectory(%project_name%) to the main CMakeLists.txt.
    pause
    exit /b
)
echo [INFO] Added add_subdirectory(%project_name%) to the main CMakeLists.txt.

echo.
echo [SUCCESS] Project "%project_name%" has been created successfully.
echo - Project directory: "%project_name%"
echo - CMakeLists.txt generated.
echo - %NEW_CPP_FILE% created in the "src" folder.
echo - Added add_subdirectory(%project_name%) to the main CMakeLists.txt.
pause
