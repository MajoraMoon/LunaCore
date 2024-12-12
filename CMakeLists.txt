# Minimum Version of CMake
cmake_minimum_required(VERSION 3.15)


# Projectname and Language used
project(LunaCore C)


# Set C standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)


include_directories(${CMAKE_SOURCE_DIR}/include)


# all C-Files in the main src-directory (so not from external libaries)
file(GLOB SOURCES "src/*.c")


# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# Set all SDL2 paths
set(SDL2_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2/cmake)
set(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2/x86_64-w64-mingw32/lib)
set(SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2/x86_64-w64-mingw32/include)

find_package(SDL2 REQUIRED CONFIG)


add_executable(app ${SOURCES})

target_include_directories(app PRIVATE ${SDL2_INCLUDE_DIR})

target_link_directories(app PRIVATE ${SDL2_LIB_DIR})

if(TARGET SDL2::SDL2main)
    target_link_libraries(app PRIVATE SDL2::SDL2main)
endif()

# SDL2-Bibliothek einbinden (dynamisch oder statisch)
target_link_libraries(app PRIVATE SDL2::SDL2)


# Copy SDL2.dll to the bin directory after the build
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_LIB_DIR}/../bin/SDL2.dll
    $<TARGET_FILE_DIR:app>)

