# README

## Dependencies
* Conan == 2.15
* Cmake >= 3.15
* For Mac make sure you have the sdk from Xcode

## Build Instructions
### MACOS
1. Clone or unzip repo
2. Run `conan install . \
  --profile:host=./.conan/profiles/macos_arm64 \
  --profile:build=./.conan/profiles/macos_arm64 \
  --build=missing`
3. `cd build/Release`
4. `cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release`
5. `cmake --build .`
6. Run the executable `./releaseInfoCLI <option>`
### LINUX
1. Clone or unzip repo
2. `conan detect profile`
3. `conan install . --build=missing`
4. `cd build/Release`
5. `cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release`
6. `cmake --build .`
7. Run the executable `./releaseInfoCLI <option>`
### WINDOWS
I don't have access to a windows machine so I can't guarantee this works

## Future Improvements
While I don't intend on improving this any further, the obvious next step would be to improve performance by caching the fetched data for future use. Pulling ~16 Mb is pretty slow and throwing it away each time is wasteful. Instead you could keep a local copy and when the program runs it could first just check the header to see if the info has been updated since last fetch. If not, you can use the local data.


