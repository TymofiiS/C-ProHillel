# ProjectManager

Minimal multi-target CMake layout with a core library, CLI app, and example.

## Build (Conan 2 + CMake)

```powershell
cd ProjectManager
conan install . -of build -s build_type=Release --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run:
```powershell
.\build\Release\project_manager.exe
.\build\Release\basic_usage.exe
```
