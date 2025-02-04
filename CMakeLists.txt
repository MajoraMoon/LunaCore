cmake_minimum_required(VERSION 3.15)


project(LunaCore C CXX)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")



# C-Data
file(GLOB_RECURSE SOURCES "src/*.c")
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c)
set_source_files_properties(${GLAD_SRC} PROPERTIES LANGUAGE C)


file(GLOB IMGUI_SOURCES 
    ${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
set_target_properties(imgui PROPERTIES LANGUAGE CXX)

# Wichtige Include-Pfade für ImGui auch für das Library-Target
target_include_directories(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/imgui
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends
)

add_executable(LunaCore ${SOURCES} ${GLAD_SRC})
target_link_libraries(LunaCore PRIVATE imgui)

target_include_directories(LunaCore PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/glad/include
    ${CMAKE_SOURCE_DIR}/include
)


# Windows specific configuriation. SDL2 is on the C-Drive.
if(WIN32)

    # set here for windows later (SDL3 SDL3_Image)

    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        C:/SDL2/x86_64-w64-mingw32/bin/SDL2.dll
        $<TARGET_FILE_DIR:app>)
    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        C:/SDL2_image/x86_64-w64-mingw32/bin/SDL2_image.dll
        $<TARGET_FILE_DIR:app>)


elseif(UNIX)

# When I'm using "SDL3::SDL3" for target_link_libraries, 
# I get an undefined reference to `SDL_WaitAndAcquireGPUSwapchainTexture'.
# Arch repos are not that fast, SDL3_image is not ther yet lol so did it manually
include_directories(/usr/local/include/SDL3_image)

find_package(SDL3 REQUIRED)
target_link_libraries(LunaCore PRIVATE /usr/lib/libSDL3.so)
target_link_libraries(LunaCore PRIVATE /usr/local/lib/libSDL3_image.so)




endif()
