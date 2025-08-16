How to lauch code tests coverage analyzing

# If execution policy blocks scripts for this session:
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force

.\tests\run_coverage_step_by_step.ps1 -NoPause
.\tests\run_coverage_step_by_step.ps1 -Auto
.\tests\run_coverage_step_by_step.ps1 -NonInteractive

# See results in in coverage folder