param([Parameter(Mandatory=$true)][string]$EngineRoot)
$ErrorActionPreference='Stop'
$exe=Join-Path $EngineRoot 'Engine/Binaries/Win64/UnrealEditor-Cmd.exe'
$project=Join-Path $PSScriptRoot 'CourseGame.uproject'
$script=Join-Path $PSScriptRoot 'Tools/SetupWeek06.py'
$flag=Join-Path $PSScriptRoot 'RESOURCES_READY.flag'
if(Test-Path -LiteralPath $flag){Remove-Item -LiteralPath $flag}
& $exe $project -run=pythonscript "-script=$script" -unattended -nullrhi -nosound
if($LASTEXITCODE -ne 0 -or !(Test-Path -LiteralPath $flag)){throw 'Resource setup failed. Read Saved/Logs before opening the Week06 map.'}
