@echo off
chcp 65001 >nul
:: Скрипт сборки Nonagon CS2 для любимого хозяина :3
title FemboyLose Builder :3

set "SOLUTION_DIR=%~dp0"
cd /d "%SOLUTION_DIR%"

echo [~] Ищу компилятор MSBuild на твоем ПК, Хозяин... // мяу :3

set "MSBUILD_PATH="

:: Проверяем через vswhere // :3
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
        set "MSBUILD_PATH=%%i"
    )
)

:: Проверяем стандартные пути VS 2022 // ня :3
if "%MSBUILD_PATH%"=="" (
    for %%p in (
        "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
    ) do (
        if exist %%p set "MSBUILD_PATH=%%~p"
    )
)

:: Если MSBuild не найден - запускаем установку через winget // uwu :3
if "%MSBUILD_PATH%"=="" (
    echo [!] Ой, Хозяин, MSBuild / Visual Studio C++ не установлены на твоем ПК! // :0
    echo [~] Скачиваю и устанавливаю Visual Studio 2022 Build Tools (C++ v143 + WinSDK)... // мяу :3
    winget install --id Microsoft.VisualStudio.2022.BuildTools --accept-package-agreements --accept-source-agreements --override "--passive --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
    
    :: Проверяем снова после установки // :3
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
        for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
            set "MSBUILD_PATH=%%i"
        )
    )
)

if "%MSBUILD_PATH%"=="" (
    echo [-] Хозяин, компилятор не найден даже после установки. Проверь установку VS 2022 с пакетом C++! // муурр T_T
    pause
    exit /b 1
)

echo [+] MSBuild найден: "%MSBUILD_PATH%" // ^_^ :3
echo [~] Начинаю компиляцию решения base.sln в конфигурации Debug ^| x64... // муурр :3

"%MSBUILD_PATH%" "%SOLUTION_DIR%base.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m /v:m

if %errorlevel% neq 0 (
    echo [-] Ошибка компиляции, мой милый хозяин! // T_T :3
    pause
    exit /b %errorlevel%
)

echo [~] Теперь компилирую конфигурацию Release ^| x64... // :3

"%MSBUILD_PATH%" "%SOLUTION_DIR%base.sln" /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /v:m

echo.
echo ========================================================
echo [+] Ураа! Билд успешно собран для моего Хозяина! // мяу :3
echo [+] Debug DLL:   "%SOLUTION_DIR%build\Debug\test.dll"
echo [+] Release DLL: "%SOLUTION_DIR%build\Release\cstrike.dll"
echo ========================================================
echo.
pause
