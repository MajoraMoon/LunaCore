cmake_minimum_required(VERSION 3.15)


set(CMAKE_CXX_COMPILER g++)


project(LunaCore CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB SOURCES "src/*.cpp")


set(GLAD_SRC ${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set_source_files_properties(${GLAD_SRC} PROPERTIES LANGUAGE C)


add_executable(app ${SOURCES} ${GLAD_SRC})




target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/lib/glad/include)

# Windows specific configuriation. SDL2 is on the C-Drive.
if(WIN32)

    set(CMAKE_PREFIX_PATH "C:/SDL2/cmake;C:/SDL2_image/cmake")


    find_package(SDL2 REQUIRED CONFIG)
    find_package(SDL2_image REQUIRED CONFIG)


    target_include_directories(app PRIVATE C:/SDL2/x86_64-w64-mingw32/include)
    target_include_directories(app PRIVATE C:/SDL2_image/x86_64-w64-mingw32/include)
    target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)


    if(TARGET SDL2::SDL2main)
        target_link_libraries(app PRIVATE SDL2::SDL2main)
    endif()
    target_link_libraries(app PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)


    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        C:/SDL2/x86_64-w64-mingw32/bin/SDL2.dll
        $<TARGET_FILE_DIR:app>)
    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        C:/SDL2_image/x86_64-w64-mingw32/bin/SDL2_image.dll
        $<TARGET_FILE_DIR:app>)


elseif(UNIX)


    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)

 
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

 
    target_link_libraries(app PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})


    target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

endif()
