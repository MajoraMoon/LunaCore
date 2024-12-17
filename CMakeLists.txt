# cmake_minimum_required gibt die Mindestversion von CMake an, die für dieses Projekt erforderlich ist.
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER g++)


project(LunaCore CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Alle C++-Dateien im `src/`-Verzeichnis hinzufügen
file(GLOB SOURCES "src/*.cpp")

# GLAD-Quellcode-Datei festlegen
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c)

# Setze das Zielverzeichnis für die Ausführungsdatei
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set_source_files_properties(${GLAD_SRC} PROPERTIES LANGUAGE C)


# Erstelle das ausführbare Programm
add_executable(app ${SOURCES} ${GLAD_SRC})

# Inkludiere das FreeType-Bibliotheksverzeichnis
#add_subdirectory(lib/freetype)

# Füge FreeType als Abhängigkeit hinzu
#target_link_libraries(app PRIVATE freetype)

# Inkludiere GLAD-Hauptverzeichnis
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/lib/glad/include)

# Windows-spezifische Konfiguration
if(WIN32)

    # Setze Pfad zu SDL2
    set(CMAKE_PREFIX_PATH "C:/SDL2/cmake")

    # Finde und lade SDL2
    find_package(SDL2 REQUIRED CONFIG)

    # Füge SDL2-Headerverzeichnisse hinzu
    target_include_directories(app PRIVATE C:/SDL2/x86_64-w64-mingw32/include)
    target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

    # Linke SDL2
    if(TARGET SDL2::SDL2main)
        target_link_libraries(app PRIVATE SDL2::SDL2main)
    endif()
    target_link_libraries(app PRIVATE SDL2::SDL2)

    # Kopiere SDL2.dll in das Bin-Verzeichnis nach dem Build
    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        C:/SDL2/x86_64-w64-mingw32/bin/SDL2.dll
        $<TARGET_FILE_DIR:app>)

# Linux-spezifische SDL2-Konfiguration
elseif(UNIX)

    # Finde und lade SDL2 für Unix-basierte Systeme
    find_package(SDL2 REQUIRED)

    # Inkludiere SDL2-Headerverzeichnisse
    include_directories(${SDL2_INCLUDE_DIRS})

    # Linke SDL2
    target_link_libraries(app PRIVATE ${SDL2_LIBRARIES})

    # Inkludiere das Hauptverzeichnis des Projekts
    target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

endif()
