[CmdletBinding()]
param(
  [ValidateSet('Debug','RelWithDebInfo','Release')][string]$Config = 'Debug',
  [string]$BuildDir = 'build',
  [string]$CoverageDir = 'coverage',
  [switch]$NoPause,
  [string]$Which  # deprecated; ignored (kept for backward compatibility)
)

$ErrorActionPreference = 'Stop'

# Resolve repo root even if launched from tests/
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$BuildPath = Join-Path $RepoRoot $BuildDir
$CoveragePath = Join-Path $RepoRoot $CoverageDir

function Pause-Step($msg) {
  if (-not $NoPause) {
    Write-Host ">>> $msg"
    Read-Host "Press Enter to continue (or Ctrl+C to abort)" | Out-Null
  } else {
    Write-Host ">>> $msg"
  }
}

function Find-TestExe([string]$BuildRoot, [string]$TargetName) {
  $exe = Get-ChildItem -Path $BuildRoot -Recurse -File -Filter "$TargetName.exe" | Select-Object -First 1
  if (-not $exe) { throw "Cannot find $TargetName.exe under '$BuildRoot'." }
  return $exe.FullName
}

function Run-Tests([string]$Target, [string[]]$Sources, [string[]]$ExcludedSources) {
  Pause-Step "Build $Target ($Config)"
  cmake --build $BuildPath --target $Target --config $Config

  Pause-Step "Locate $Target.exe"
  $testExe = Find-TestExe -BuildRoot $BuildPath -TargetName $Target
  Write-Host "Found: $testExe" -ForegroundColor Green

  Pause-Step "Run $Target WITHOUT coverage"
  & $testExe --gtest_color=yes --gtest_print_time=1

  $occ = Get-Command OpenCppCoverage -ErrorAction SilentlyContinue
  if ($null -eq $occ) {
    Write-Warning "OpenCppCoverage not found in PATH; skipping coverage for $Target."
    return
  }

  New-Item -ItemType Directory -Force -Path $CoveragePath | Out-Null
  $outHtml = Join-Path $CoveragePath ("{0}_{1}.html" -f $Target, ($Config.ToLower()))

  Pause-Step "Run OpenCppCoverage for $Target"
  $args = @("--cover_children", "--modules", "$Target.exe")
  foreach ($s in $Sources)         { $args += @("--sources", $s) }
  foreach ($x in $ExcludedSources) { $args += @("--excluded_sources", $x) }
  $args += @("--export_type", "html:$outHtml", "--", $testExe, "--gtest_color=yes", "--gtest_print_time=1")

  & OpenCppCoverage @args
  Write-Host "Coverage report: $outHtml" -ForegroundColor Cyan
}

Write-Host "=== ProjectManager: build+test+coverage (core then app) ===" -ForegroundColor Cyan
Write-Host "Config: $Config"
Write-Host "RepoRoot: $RepoRoot"
Write-Host "Build: $BuildPath"
Write-Host "Coverage: $CoveragePath"

if (-not (Test-Path $BuildPath)) { New-Item -ItemType Directory -Path $BuildPath | Out-Null }

Pause-Step "Conan install ($Config)"
conan install $RepoRoot -of $BuildPath -s build_type=$Config --build=missing

Pause-Step "CMake configure ($Config)"
cmake -S $RepoRoot -B $BuildPath -DCMAKE_TOOLCHAIN_FILE="$BuildPath/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=$Config

# Source filters
$srcCore  = (Resolve-Path (Join-Path $RepoRoot 'src\core')).Path
$srcApp   = (Resolve-Path (Join-Path $RepoRoot 'src\app')).Path
$srcTests = (Resolve-Path (Join-Path $RepoRoot 'tests')).Path

# 1) CORE TESTS
Run-Tests -Target "core_tests" -Sources @($srcCore) -ExcludedSources @($srcTests)

# 2) APP (CLI) TESTS
Run-Tests -Target "app_tests" -Sources @($srcCore, $srcApp) -ExcludedSources @($srcTests)

Write-Host "All done." -ForegroundColor Green
