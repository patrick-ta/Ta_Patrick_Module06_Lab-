param([switch]$Baseline)
$ErrorActionPreference='Stop'
$compiler=Get-Command cl.exe -ErrorAction SilentlyContinue
if(!$compiler){throw 'Run this in the Visual Studio Developer PowerShell, where cl.exe is available.'}
$build=Join-Path $PSScriptRoot 'Saved/RulesTests';New-Item -ItemType Directory -Force -Path $build | Out-Null
$src=Join-Path $PSScriptRoot 'Tests/StateRulesTests.cpp'
$inc=Join-Path $PSScriptRoot 'Source/CourseGame/Week06'
$exe=Join-Path $build 'StateRulesTests.exe'
& cl.exe /nologo /EHsc /std:c++17 "/I$inc" $src "/Fe:$exe" "/Fo:$build/StateRulesTests.obj"
if($LASTEXITCODE -ne 0){throw 'Rules compilation failed'}
if($Baseline){& $exe baseline}else{& $exe}
if($LASTEXITCODE -ne 0){throw 'Rule checks failed (expected for unfinished student starter without -Baseline)'}
