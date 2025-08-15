How to lauch code examples

# optional: allow scripts for this session
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force

# from repo root or examples/
.\examples\run_demo_step_by_step.ps1                  # Debug, build/
.\examples\run_demo_step_by_step.ps1 -Config Release  # Release
.\examples\run_demo_step_by_step.ps1 -BuildDir out\build
.\examples\run_demo_step_by_step.ps1 -NoPause         # no step prompts