# tool_box

A small Windows toolbox written in C++20.

## Build

Prerequisites: MinGW-w64 (GCC 13+), CMake 3.21+, Ninja, vcpkg.

```powershell
cmake --preset mingw-debug
cmake --build --preset debug
```

## Run

```powershell
.\build\debug\tool_box.exe -n Alice
```