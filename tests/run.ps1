param([string]$Python = 'C:\Users\soraa\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe')
$ErrorActionPreference='Stop'
$project=Split-Path $PSScriptRoot -Parent
Push-Location $project
try {
    & tools/build.ps1
    $compiler=Join-Path $project 'research/toolchain/w64devkit/bin/g++.exe'
    & $compiler -std=c++17 -static -O2 -Wall -Wextra -Werror -Wno-misleading-indentation tests/offline.cpp -o build/offline-tests.exe -luser32
    if($LASTEXITCODE){throw 'Offline tests failed to compile'}
    & build/offline-tests.exe
    if($LASTEXITCODE){throw 'Offline tests failed'}
    New-Item -ItemType Directory -Force build/loader-test | Out-Null
    & $compiler -shared -static -O2 tests/shv-stub.cpp tests/shv-stub.def -o build/loader-test/ScriptHookV.dll
    if($LASTEXITCODE){throw 'Mock runtime failed to compile'}
    Copy-Item -LiteralPath build/SoraMod.asi -Destination build/loader-test/SoraMod.asi -Force
    & $Python tools/verify.py
    if($LASTEXITCODE){throw 'Asset/loader verification failed'}
} finally {Pop-Location}
