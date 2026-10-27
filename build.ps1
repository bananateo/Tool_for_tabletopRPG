# Add the toolchain, CMake, Ninja and Qt's DLLs to PATH for this session
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;C:\Qt\6.12.0\mingw_64\bin;" + $env:PATH

# Configure and build
cd $PSScriptRoot
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_C_COMPILER="C:/Qt/Tools/mingw1310_64/bin/gcc.exe"
cmake --build build

# Run
.\build\MyQtApp.exe