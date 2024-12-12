# The comments are coming from me, because I'm learning Cmake and all that stuff. I know some of these comments may seem dumb.

cmake_minimum_required(VERSION 3.15)
project(LunaCore C)

# Set C standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include all C-Files in the src-directory
file(GLOB SOURCES "src/*.c")

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Set SDL2 paths
set(CMAKE_PREFIX_PATH "C:/SDL2/cmake")
find_package(SDL2 REQUIRED CONFIG)

# Add executable
add_executable(app ${SOURCES})

# Include SDL2 headers
target_include_directories(app PRIVATE C:/SDL2/x86_64-w64-mingw32/include)

# Link SDL2 (seems not really necessary but for let it's just a chill if clause)
if(TARGET SDL2::SDL2main)
    target_link_libraries(app PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(app PRIVATE SDL2::SDL2)


# Copy SDL2.dll to the bin directory after the build
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    C:/SDL2/x86_64-w64-mingw32/bin/SDL2.dll
    $<TARGET_FILE_DIR:app>)