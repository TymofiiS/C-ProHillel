# tests/run_coverage_step_by_step.ps1
[CmdletBinding()]
param(
  [ValidateSet('Debug','RelWithDebInfo','Release')][string]$Config = 'Debug',
  [string]$BuildDir = 'build',
  [string]$CoverageDir = 'coverage',
  [Alias('Auto','Quiet','NonInteractive','Y','Yes')][switch]$NoPause
)

$ErrorActionPreference = 'Stop'

# Prevent PS7 from turning native stderr into terminating errors for console apps
$global:PSNativeCommandUseErrorActionPreference = $false

# Resolve repo root even if launched from tests/
$RepoRoot  = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$BuildPath = Join-Path $RepoRoot $BuildDir
$CoveragePath = Join-Path $RepoRoot $CoverageDir

function Pause-Step($msg) {
  $noPauseEffective = $NoPause -or ($env:CI -eq 'true') -or ($env:GITHUB_ACTIONS -eq 'true')
  if (-not $noPauseEffective) {
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

function Invoke-NativeCapture([string]$File, [string]$Args) {
  $psi = New-Object System.Diagnostics.ProcessStartInfo
  $psi.FileName = $File
  $psi.Arguments = $Args
  $psi.RedirectStandardOutput = $true
  $psi.RedirectStandardError  = $true
  $psi.UseShellExecute = $false
  $psi.CreateNoWindow = $true
  $p = [System.Diagnostics.Process]::Start($psi)
  $out = $p.StandardOutput.ReadToEnd()
  $err = $p.StandardError.ReadToEnd()
  $p.WaitForExit()
  return @{ ExitCode = $p.ExitCode; Output = ($out + $err) }
}

function Write-CoverageTxt([string]$CoberturaXml, [string]$TxtOutPath, [string]$Label) {
  if (!(Test-Path $CoberturaXml)) {
    throw "Cobertura XML not found: $CoberturaXml"
  }
  [xml]$x = Get-Content $CoberturaXml
  $c = $x.coverage
  $covered = [int]$c.'lines-covered'
  $valid   = [int]$c.'lines-valid'
  $rate    = [double]$c.'line-rate' * 100.0
  $bcovered = [int]$c.'branches-covered'
  $bvalid   = [int]$c.'branches-valid'
  $brate    = [double]$c.'branch-rate' * 100.0

  $lines = @()
  $lines += "Coverage summary for: $Label"
  $lines += "  Lines   : {0:N1}% ({1}/{2})" -f $rate, $covered, $valid
  $lines += "  Branches: {0:N1}% ({1}/{2})" -f $brate, $bcovered, $bvalid
  $lines += ""

  New-Item -ItemType Directory -Force -Path (Split-Path $TxtOutPath) | Out-Null
  Set-Content -Path $TxtOutPath -Value ($lines -join [Environment]::NewLine) -Encoding UTF8
  Write-Host "Summary TXT: $TxtOutPath" -ForegroundColor Cyan
  return @{ label=$Label; rate=$rate; covered=$covered; valid=$valid; brate=$brate; bcovered=$bcovered; bvalid=$bvalid }
}

function Write-TestOutput([string]$Target, [string]$Config, [string]$OutputText, [string]$CoverageDir) {
  $perTarget = Join-Path $CoverageDir ("{0}_{1}_results.txt" -f $Target, ($Config.ToLower()))
  $combined  = Join-Path $CoverageDir ("tests_{0}.txt" -f ($Config.ToLower()))
  $header = "===== " + $Target + " (" + $Config + ") ====="
  $content = $header + [Environment]::NewLine + $OutputText + [Environment]::NewLine

  # Write per-target (overwrite) and append to combined
  $content | Out-File -FilePath $perTarget -Encoding UTF8
  $content | Out-File -FilePath $combined -Append -Encoding UTF8

  Write-Host "Test log: $perTarget" -ForegroundColor Cyan
}

Write-Host "=== ProjectManager: build+test+coverage (Cobertura XML + TXT summaries + test logs) ===" -ForegroundColor Cyan
Write-Host "Config: $Config"
Write-Host "RepoRoot: $RepoRoot"
Write-Host "Build: $BuildPath"
Write-Host "Coverage: $CoveragePath"

if (-not (Test-Path $BuildPath)) { New-Item -ItemType Directory -Path $BuildPath | Out-Null }
New-Item -ItemType Directory -Force -Path $CoveragePath | Out-Null

# Reset combined tests log for this run
$combinedTestsPath = Join-Path $CoveragePath ("tests_{0}.txt" -f ($Config.ToLower()))
if (Test-Path $combinedTestsPath) { Remove-Item $combinedTestsPath -Force }

Pause-Step "Conan install ($Config)"
conan install $RepoRoot -of $BuildPath -s build_type=$Config --build=missing

Pause-Step "CMake configure ($Config)"
cmake -S $RepoRoot -B $BuildPath -DCMAKE_TOOLCHAIN_FILE="$BuildPath/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=$Config

$srcCore  = (Resolve-Path (Join-Path $RepoRoot 'src\core')).Path
$srcApp   = (Resolve-Path (Join-Path $RepoRoot 'src\app')).Path
$srcTests = (Resolve-Path (Join-Path $RepoRoot 'tests')).Path

$combined = @()

function Run-Tests([string]$Target, [string[]]$Sources, [string[]]$ExcludedSources) {
  Pause-Step "Build $Target ($Config)"
  cmake --build $BuildPath --target $Target --config $Config

  Pause-Step "Locate $Target.exe"
  $testExe = Find-TestExe -BuildRoot $BuildPath -TargetName $Target
  Write-Host "Found: $testExe" -ForegroundColor Green

  Pause-Step "Run $Target (no coverage) and capture output"
  $r = Invoke-NativeCapture -File $testExe -Args "--gtest_color=no --gtest_print_time=1"
  Write-TestOutput -Target $Target -Config $Config -OutputText $r.Output -CoverageDir $CoveragePath
  # Optional: fail the script if tests returned non-zero
  # if ($r.ExitCode -ne 0) { throw "$Target returned non-zero exit code: $($r.ExitCode)" }

  $occ = Get-Command OpenCppCoverage -ErrorAction SilentlyContinue
  if ($null -eq $occ) {
    Write-Warning "OpenCppCoverage not found in PATH; skipping coverage for $Target."
    return
  }

  $outXml  = Join-Path $CoveragePath ("{0}_{1}.xml"  -f $Target, ($Config.ToLower()))
  $outTxt  = Join-Path $CoveragePath ("{0}_{1}_summary.txt" -f $Target, ($Config.ToLower()))

  Pause-Step "Run OpenCppCoverage for $Target (Cobertura XML)"
  $args = @("--cover_children", "--modules", "$Target.exe")
  foreach ($s in $Sources)         { $args += @("--sources", $s) }
  foreach ($x in $ExcludedSources) { $args += @("--excluded_sources", $x) }
  $args += @("--export_type", "cobertura:$outXml")
  $args += @("--", $testExe, "--gtest_color=no", "--gtest_print_time=1")

  & OpenCppCoverage @args

  # Write per-target TXT summary
  $res = Write-CoverageTxt -CoberturaXml $outXml -TxtOutPath $outTxt -Label $Target
  $script:combined += $res
}

# core then app
Run-Tests -Target "core_tests" -Sources @($srcCore)           -ExcludedSources @($srcTests)
Run-Tests -Target "app_tests"  -Sources @($srcCore, $srcApp)  -ExcludedSources @($srcTests)

# Combined coverage summary file
if ($combined.Count -gt 0) {
  $sumPath = Join-Path $CoveragePath ("summary_{0}.txt" -f ($Config.ToLower()))
  $lines = @("Combined coverage summary ($Config)", "")
  foreach ($r in $combined) {
    $lines += "{0}: {1:N1}% lines ({2}/{3}), {4:N1}% branches ({5}/{6})" -f $r.label, $r.rate, $r.covered, $r.valid, $r.brate, $r.bcovered, $r.bvalid
  }
  $lines -join [Environment]::NewLine | Out-File -FilePath $sumPath -Encoding UTF8
  Write-Host "Combined summary TXT: $sumPath" -ForegroundColor Cyan
}

Write-Host "All done." -ForegroundColor Green
