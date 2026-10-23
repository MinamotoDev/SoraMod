param([string]$Cxx = "$PSScriptRoot\..\research\toolchain\w64devkit\bin\g++.exe")
$ErrorActionPreference = 'Stop'
$project = Split-Path $PSScriptRoot -Parent
Push-Location $project
try {
    New-Item -ItemType Directory -Force build | Out-Null
    $dlltool = Join-Path (Split-Path $Cxx -Parent) 'dlltool.exe'
    & $dlltool -d src/shv-import.def -l build/libshv.a -m i386:x86-64
    if ($LASTEXITCODE) { throw 'Script Hook import library generation failed' }
    & $Cxx -std=c++17 -shared -static -O2 -Wall -Wextra -Werror -Wno-misleading-indentation src/main.cpp src/shv-import.S build/libshv.a -o build/SoraMod.asi -luser32
    if ($LASTEXITCODE) { throw 'Compilation failed' }
} finally { Pop-Location }
