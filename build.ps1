# Скрипт сборки Nonagon CS2 для любимого хозяина :3
Write-Host "[~] Ищу компилятор MSBuild на твоем ПК, Хозяин..." -ForegroundColor Cyan # мяу :3

$solutionDir = $PSScriptRoot
if (-not $solutionDir) { $solutionDir = Get-Location }
$slnPath = Join-Path $solutionDir "base.sln"

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$msbuild = $null

if (Test-Path $vswhere) {
    $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
}

if (-not $msbuild) {
    $candidatePaths = @(
        "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
    )
    foreach ($path in $candidatePaths) {
        if (Test-Path $path) {
            $msbuild = $path
            break
        }
    }
}

if (-not $msbuild) {
    Write-Host "[!] Хозяин, MSBuild / Visual Studio C++ не установлены!" -ForegroundColor Yellow # :0
    Write-Host "[~] Скачиваю Visual Studio 2022 Build Tools (C++ v143 + WinSDK) через winget..." -ForegroundColor Magenta # мяу :3
    winget install --id Microsoft.VisualStudio.2022.BuildTools --accept-package-agreements --accept-source-agreements --override "--passive --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
    
    if (Test-Path $vswhere) {
        $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
    }
}

if (-not $msbuild) {
    Write-Host "[-] Ой, Хозяин, компилятор не найден. Пожалуйста, установи Visual Studio 2022 с C++ пакетом!" -ForegroundColor Red # муурр T_T
    exit 1
}

Write-Host "[+] MSBuild найден: $msbuild" -ForegroundColor Green # ^_^ :3
Write-Host "[~] Запускаю сборку Debug | x64..." -ForegroundColor Cyan # :3

& $msbuild $slnPath /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m /v:m

if ($LASTEXITCODE -ne 0) {
    Write-Host "[-] Ошибка при сборке Debug, Хозяин!" -ForegroundColor Red # T_T :3
    exit $LASTEXITCODE
}

Write-Host "[~] Запускаю сборку Release | x64..." -ForegroundColor Cyan # uwu :3
& $msbuild $slnPath /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /v:m

Write-Host "`n[+] Готово, Хозяин! DLL скомпилированы:" -ForegroundColor Green # мяу :3
Write-Host "    Debug:   $solutionDir\build\Debug\test.dll" -ForegroundColor Yellow
Write-Host "    Release: $solutionDir\build\Release\cstrike.dll" -ForegroundColor Yellow
