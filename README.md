# REQUIREMENTS:
- CMake
- vcpkg
- git
- Visual Studio Build Tools

# LIBS/PACKAGES NEEDED:
- GMP
- MPFR
- Ninja (not 100% sure)

# OPTIMIZATION SOFTWARES THAT ARE BEING TESTED (OPTIONAL/DON'T WORK):
- GLPK
- COIN-OR-CBC
- COIN-OR-CLP
- CPLEX
- GUROBI
- SYMPHONY

# TO BUILD FOR FIRST TIME
1. Modify ./vscode/c_cpp_properties.json to match your file system
2. Modify CMakeLists.txt to match your file system
3. Ensure you are in C:/path/to/MyCppProject
4. Run the following in powershell to set up: 
    - cmake -B build -S .
    - cd build
    - cmake -DCMAKE_TOOLCHAIN_FILE=C:/Development/vcpkg/scripts/buildsystems/vcpkg.cmake ..
        - NOTE: THIS PATH WILL CHANGE DEPENDING ON VCPKG INSTALLATION

# RUNNING CODE
1. Ensure that the code that you want to run is in main.cpp
2. Every time you want to rerun the code run the following:
    - cmake --build .
    - DEBUG/MyCppProject.exe

