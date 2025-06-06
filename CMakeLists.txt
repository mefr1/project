cmake_minimum_required(VERSION 3.10)
project(WorkoutTrackerGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# 1) Находим OpenGL
# ------------------------------------------------------------------------------
find_package(OpenGL REQUIRED)

# ------------------------------------------------------------------------------
# 2) Находим GLFW
# ------------------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
if(NOT GLFW_FOUND)
    message(FATAL_ERROR "GLFW3 not found. Установите libglfw3-dev")
endif()

# ------------------------------------------------------------------------------
# 3) Находим GLEW
# ------------------------------------------------------------------------------
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found. Установите libglew-dev")
endif()

# ------------------------------------------------------------------------------
# 4) Подключаем подпапку imgui, где лежит CMakeLists.txt (см. выше)
# ------------------------------------------------------------------------------
add_subdirectory(imgui)

# ------------------------------------------------------------------------------
# 5) Список ваших исходников и заголовков
# ------------------------------------------------------------------------------
set(SOURCES
    main_gui.cpp
    Workout.cpp
    WorkoutTracker.cpp
    Exercise.cpp
)

set(HEADERS
    Workout.h
    WorkoutTracker.h
    Exercise.h
)

# ------------------------------------------------------------------------------
# 6) Собираем исполняемый таргет
# ------------------------------------------------------------------------------
add_executable(WorkoutTrackerGUI ${SOURCES} ${HEADERS})

# ------------------------------------------------------------------------------
# 7) Указываем include-директории (GLFW, GLEW сами дают include-пути через find_package)
# ------------------------------------------------------------------------------
target_include_directories(WorkoutTrackerGUI
    PRIVATE
        ${GLFW_INCLUDE_DIRS}   # из pkg_search_module(GLFW ...)
        ${GLEW_INCLUDE_DIRS}   # из find_package(GLEW ...)
)

# ------------------------------------------------------------------------------
# 8) Линкуем с нужными библиотеками
# ------------------------------------------------------------------------------
target_link_libraries(WorkoutTrackerGUI
    PRIVATE
        imgui                 # статическая библиотека ImGui из imgui/CMakeLists.txt
        ${GLFW_LIBRARIES}     # из pkg_search_module(GLFW ...)
        ${OPENGL_gl_LIBRARY}  # из find_package(OpenGL ...)
        GLEW::GLEW            # из find_package(GLEW ...)
)

# ------------------------------------------------------------------------------
# 9) Если нужно, можно задать дополнительные опции (но обычно этого достаточно)
# ------------------------------------------------------------------------------
