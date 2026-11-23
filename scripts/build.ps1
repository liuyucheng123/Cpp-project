# Compilation et tests du projet Codex Naturalis.
# Le compilateur fourni avec CLion (MinGW) est utilise directement : aucune
# dependance externe, donc la compilation fonctionne meme hors de l'IDE.
#   powershell -ExecutionPolicy Bypass -File scripts\build.ps1
$ErrorActionPreference = "Stop"

$racine = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$gpp = "D:\CLion 2026.2.3\bin\mingw\bin\g++.exe"
if (-not (Test-Path $gpp)) {
    $gpp = "g++.exe"
}
$dossierBin = Split-Path -Parent (Get-Command $gpp).Source
$env:PATH = $dossierBin + ";" + $env:PATH

$build = Join-Path $racine "build"
New-Item -ItemType Directory -Force -Path $build | Out-Null

$options = @("-std=c++14", "-Wall", "-Wextra", "-pedantic",
             "-static-libgcc", "-static-libstdc++",
             "-I", (Join-Path $racine "src"))

$sources = @(Get-ChildItem -Path (Join-Path $racine "src") -Recurse -Filter *.cpp |
             ForEach-Object { $_.FullName })
$sourcesTests = @($sources | Where-Object { $_ -notlike "*main.cpp" })

$exe = Join-Path $build "codex_naturalis.exe"
Write-Host "Compilation du jeu..." -ForegroundColor Cyan
& $gpp @options @sources -o $exe
if ($LASTEXITCODE -ne 0) { throw "compilation du jeu en echec" }

$dll = Join-Path $dossierBin "libwinpthread-1.dll"
if (Test-Path $dll) { Copy-Item $dll $build -Force }

Write-Host "Compilation des tests..." -ForegroundColor Cyan
$tests = @(Get-ChildItem -Path (Join-Path $racine "tests") -Filter *.cpp |
           ForEach-Object { $_.FullName })
$exeTests = Join-Path $build "codex_tests.exe"
& $gpp @options "-I" (Join-Path $racine "tests") @sourcesTests @tests -o $exeTests
if ($LASTEXITCODE -ne 0) { throw "compilation des tests en echec" }

Write-Host "Execution des tests..." -ForegroundColor Cyan
& $exeTests
if ($LASTEXITCODE -ne 0) { throw "tests en echec" }

Write-Host ""
Write-Host "Build OK : $exe" -ForegroundColor Green
Write-Host "           $exeTests" -ForegroundColor Green
