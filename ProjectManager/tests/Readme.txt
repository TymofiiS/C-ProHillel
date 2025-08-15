How to lauch code tests coverage analyzing

# If execution policy blocks scripts for this session:
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force

# Run for core tests (Debug)
.\tests\run_coverage_step_by_step.ps1 -Config Debug

# See results in *.html files in coverage folder