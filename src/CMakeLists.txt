# Setze das Projekt für das `src/`-Verzeichnis
project(CGP_project)

# Setze den C++-Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setze die Include-Pfade für OpenGL und externe Bibliotheken
include_directories(C:/OpenGLlibraries/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Setze die Library-Pfade
link_directories(C:/OpenGLlibraries/lib)

# Finde alle `mainX.cpp`-Dateien automatisch
file(GLOB TASK_FILES "main*.cpp")

# Für jede gefundene Datei wird ein Executable erstellt
foreach(FILE ${TASK_FILES})
    get_filename_component(TASK_NAME ${FILE} NAME_WE)
    add_executable(${TASK_NAME} ${FILE} ${CMAKE_SOURCE_DIR}/src/Utils.cpp)

    # Verlinke OpenGL, GLFW, GLEW und SOIL2
    target_link_libraries(${TASK_NAME} glfw3 glew32 soil2-debug opengl32)
endforeach()
