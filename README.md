# ED_Fishing

ED_Fishing

## 编译
```sh
// WEB
cmake -D CMAKE_TOOLCHAIN_FILE=D:/cpp/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -G "Ninja" -D CMAKE_BUILD_TYPE=Release -S "./" -B "./emscripten"
// EXE
cmake -D CMAKE_TOOLCHAIN_FILE=D:/Microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Release -D VCPKG_TARGET_TRIPLET=x64-windows-static -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON -S "./" -B "./build"
```

## 发布
```sh
// WEB
cd emscripten && emmake ninja
// EXE
cd build && cmake --build . --config Release
```


