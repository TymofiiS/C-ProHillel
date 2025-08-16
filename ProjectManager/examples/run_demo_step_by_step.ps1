
[CmdletBinding()]
param(
  [ValidateSet('Debug','RelWithDebInfo','Release')][string]$Config = 'Debug',
  [string]$BuildDir = 'build',
  [string[]]$Args = @(),
  [switch]$NoPause
)

$ErrorActionPreference = 'Stop'

# Resolve repo root even if launched from examples/
$RepoRoot  = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$BuildPath = Join-Path $RepoRoot $BuildDir

function Pause-Step($msg) {
  if (-not $NoPause) {
    Write-Host ">>> $msg"
    Read-Host "Press Enter to continue (or Ctrl+C to abort)" | Out-Null
  } else {
    Write-Host ">>> $msg"
  }
}

function Find-Exe([string]$BuildRoot, [string]$TargetName) {
  $exe = Get-ChildItem -Path $BuildRoot -Recurse -File -Filter "$TargetName.exe" | Select-Object -First 1
  if (-not $exe) { throw "Cannot find $TargetName.exe under '$BuildRoot'." }
  return $exe.FullName
}

Write-Host "=== ProjectManager: build+run DEMO (basic_usage) ===" -ForegroundColor Cyan
Write-Host "Config: $Config"
Write-Host "RepoRoot: $RepoRoot"
Write-Host "Build: $BuildPath"

if (-not (Test-Path $BuildPath)) { New-Item -ItemType Directory -Path $BuildPath | Out-Null }

Pause-Step "Conan install ($Config)"
conan install $RepoRoot -of $BuildPath -s build_type=$Config --build=missing

Pause-Step "CMake configure ($Config)"
cmake -S $RepoRoot -B $BuildPath -DCMAKE_TOOLCHAIN_FILE="$BuildPath/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=$Config

# Build example target
$target = "basic_usage"
Pause-Step "Build $target ($Config)"
cmake --build $BuildPath --target $target --config $Config

# Locate exe
Pause-Step "Locate $target.exe"
$exe = Find-Exe -BuildRoot $BuildPath -TargetName $target
Write-Host "Found: $exe" -ForegroundColor Green

# Run
Pause-Step "Run $target"
& $exe @Args
if ($LASTEXITCODE -ne 0) {
  throw "$target returned non-zero exit code: $LASTEXITCODE"
}

Write-Host "Demo completed successfully." -ForegroundColor Green
