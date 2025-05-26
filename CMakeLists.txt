cmake_minimum_required(VERSION 3.16)
project(WorkoutTrackerConsole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(workoutlib STATIC
    Exercise.cpp
    Workout.cpp
    WorkoutTracker.cpp
)

target_include_directories(workoutlib PUBLIC .)

add_executable(WorkoutTrackerConsole
    main.cpp
)

target_link_libraries(WorkoutTrackerConsole PRIVATE workoutlib)
