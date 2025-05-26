#pragma once
#include <vector>
#include "Workout.h"

class WorkoutTracker {
    std::vector<Workout> workouts;

public:
    void addWorkout(const Workout& w);
    void showAllWorkouts() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};
