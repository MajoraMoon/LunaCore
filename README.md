# Just Messing Around with Graphics in C

### _SDL2 Problem Solved:_

##### 1. installed SDL2 on the C Drive (Copied the Mingw package on to the C drive)

- I installed SDL2 on the **C Drive**:
  ```plaintext
  C:/SDL2/...
  ```
- If anyone faces the same issue as me, where CMake couldn’t find the "bin" and "lib" paths in `C:\SDL2\x86_64-w64-mingw32` (or the 32-bit version), here’s the solution… somehow.

##### 2. **Manual Installation and Environment Setup**

- I’m using **MinGW-w64**, which I manually copied to my C Drive and configured my environment variables accordingly.
- When I tried to build all files with CMake, I got an error originating from the file `C:\SDL2\x86_64-w64-mingw32\lib\cmake\SDL2\sdl2-config.cmake`.

##### 3. **Path Changes**

- I changed the paths from the defaults to ones that SDL could find itself:
  ```cmake
  set(exec_prefix "${prefix}")
  set(bindir "/tmp/tardir/SDL2-2.30.10/build-mingw/install-i686-w64-mingw32/bin")
  set(libdir "/tmp/tardir/SDL2-2.30.10/build-mingw/install-i686-w64-mingw32/lib")
  set(includedir "/tmp/tardir/SDL2-2.30.10/build-mingw/install-i686-w64-mingw32/include")
  ```
- To these paths:

  ```cmake
  set(exec_prefix "${prefix}")
  set(bindir "C:/SDL2/x86_64-w64-mingw32/bin")
  set(libdir "C:/SDL2/x86_64-w64-mingw32/lib")
  set(includedir "C:/SDL2/x86_64-w64-mingw32/include")
  ```

##### 4. **CMake File and Internal Adjustments for Smooth Running**

- With this combination of my CMake file in my repository and changes in the internal CMake file, everything runs perfectly.
