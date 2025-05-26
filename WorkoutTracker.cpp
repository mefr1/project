#include "WorkoutTracker.h"
#include <iostream>

void WorkoutTracker::addWorkout(const Workout& w) {
    workouts.push_back(w);
}

void WorkoutTracker::showAllWorkouts() const {
    for (const auto& w : workouts) {
        w.showWorkout();
        std::cout << "------------------\n";
    }
}
