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

target_include_directories(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/imgui
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends
)

add_library(imgui_c STATIC src/imgui_c.cpp)
set_target_properties(imgui_c PROPERTIES LANGUAGE CXX)

target_include_directories(imgui_c PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/imgui
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends
    ${CMAKE_SOURCE_DIR}/include      
)

# first imgui_c and then imgui otherwise this whole thing explodes
add_executable(LunaCore ${SOURCES} ${GLAD_SRC})
target_link_libraries(LunaCore PRIVATE imgui_c imgui )




target_include_directories(LunaCore PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/glad/include
    ${CMAKE_SOURCE_DIR}/include
)


# Windows specific configuriation. SDL3 is on the C-Drive.
if(WIN32)

set(CMAKE_PREFIX_PATH "C:/SDL3/cmake;C:/SDL3_image/cmake")


target_include_directories(LunaCore PRIVATE C:/SDL3/x86_64-w64-mingw32/include)
find_package(SDL3 REQUIRED CONFIG)

target_include_directories(LunaCore PRIVATE ${SDL3_INCLUDE_DIRS})

target_include_directories(LunaCore PRIVATE C:/SDL3_image/x86_64-w64-mingw32/include)
find_package(SDL3_image REQUIRED CONFIG)

set(SDL3_INCLUDE_DIR "C:/SDL3/x86_64-w64-mingw32/include")
target_include_directories(imgui PRIVATE ${SDL3_INCLUDE_DIR})
target_include_directories(imgui_c PRIVATE ${SDL3_INCLUDE_DIR})



target_link_libraries(LunaCore PRIVATE "C:/SDL3/x86_64-w64-mingw32/bin/SDL3.dll")
target_link_libraries(LunaCore PRIVATE "C:/SDL3_image/x86_64-w64-mingw32/bin/SDL3_image.dll")


    add_custom_command(TARGET LunaCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/SDL3/x86_64-w64-mingw32/bin/SDL3.dll"
        $<TARGET_FILE_DIR:LunaCore>)
    add_custom_command(TARGET LunaCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/SDL3_image/x86_64-w64-mingw32/bin/SDL3_image.dll"
        $<TARGET_FILE_DIR:LunaCore>)


elseif(UNIX)

# When I'm using "SDL3::SDL3" for target_link_libraries, 
# I get an undefined reference to `SDL_WaitAndAcquireGPUSwapchainTexture'.
# Arch repos are not that fast, SDL3_image is not ther yet lol so did it manually
include_directories(/usr/local/include/SDL3_image)

find_package(SDL3 REQUIRED)
target_link_libraries(LunaCore PRIVATE /usr/lib/libSDL3.so)
target_link_libraries(LunaCore PRIVATE /usr/local/lib/libSDL3_image.so)




endif()
